NAME			:=	philo

MAKEFLAGS		+=	-j
COMPILER		:=	cc
CFLAGS			:=	-pthread										\
					-Wall -Wextra -Werror							\
					-Wpedantic -Wundef -Wstrict-prototypes			\
					-Wshadow -Wconversion -Wsign-conversion			\
					-Wformat=2 -Wuninitialized -Wunreachable-code	\
					-MMD -MP

OPTIMIZATION	:=	-flto -Ofast
SECURITY		:=	-D_FORTIFY_SOURCE=2 -fstack-protector-strong
DEBUG_FLAGS		:=	-g3 -fno-omit-frame-pointer
SANITIZERS		:=	-fsanitize=address,undefined,null,leak,integer-divide-by-zero,signed-integer-overflow,bounds,alignment
CFLAGS			+=	$(OPTIMIZATION) $(SECURITY)
ifneq ($(filter debug,$(MAKECMDGOALS)),)
	CFLAGS		+=	$(DEBUG_FLAGS) $(SANITIZERS)
endif

ifeq ($(filter time,$(MAKECMDGOALS)),time)
	CFLAGS += -D TIME=1000
endif

ifeq ($(filter readable,$(MAKECMDGOALS)),readable)
	CFLAGS += -D READABLE=true
endif

ifeq ($(filter no_rules,$(MAKECMDGOALS)),no_rules)
	CFLAGS += -D EXPLICIT_RULES=false
endif

ifeq ($(filter debug_philo,$(MAKECMDGOALS)),debug_philo)
    CFLAGS += -D DEBUG=true
endif

ifeq ($(filter valgrind,$(MAKECMDGOALS)),valgrind)
	CFLAGS += -D VALGRIND_MARGIN=true
endif

PRINT_NO_DIR	:=	--no-print-directory
RM				:=	rm -rf

SRC_DIR			:=	src
INC_DIR			:=	include
BUILD_DIR		:=	.build

EXT_INC			:=	$(EXT_LIB)/$(INC_DIR)

SRC				=	main.c						initialize.c					init_rules.c		\
					atoi_phil.c					init_pthreads_and_mutexes.c		time.c				\
					routine.c					activity.c						supervisor_thread.c	\
					print.c	clean_up.c			reporter_of_nodes_thread.c

# Generate source file names
SRC				:=	$(addprefix $(SRC_DIR)/, $(SRC))

# Generate object file names
OBJ				:=	$(SRC:%.c=$(BUILD_DIR)/%.o)

# Generate Dependency files
DEPS			:=	$(OBJ:.o=.d)

DELETE			:=	*.out			**/*.out			.DS_Store	\
					**/.DS_Store	.dSYM/				**/.dSYM/

INCLUDES		:=	-I $(INC_DIR)
BUILD			:=	$(COMPILER) $(INCLUDES) $(CFLAGS)

all: $(NAME)

$(NAME): $(OBJ)
	@$(BUILD) $(OBJ) -o $(NAME)
	@printf "$(CREATED)" $@ $(CUR_DIR)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(BUILD) -c $< -o $@

time: all

readable: all

no_rules: all

debug_philo: all

# valgrind --tool=helgrind --max-threads=10000 ./philo
valgrind: all

clean:
	@$(RM) $(BUILD_DIR) $(DELETE)
	@printf "$(REMOVED)" $(BUILD_DIR) $(CUR_DIR)$(BUILD_DIR)

fclean: clean
	@$(RM) $(NAME)
	@printf "$(REMOVED)" $(NAME) $(CUR_DIR)

re:
	$(MAKE) $(PRINT_NO_DIR) fclean
	$(MAKE) $(PRINT_NO_DIR) all

debug: all

print-%:
	$(info $($*))

-include $(DEPS)

.PHONY:	all time readable debug_philo no_rules valgrind clean fclean re debug print-%

# Terminal markup
BOLD			:=	\033[1m
GREEN			:=	\033[32m
MAGENTA			:=	\033[35m
CYAN			:=	\033[36m
RESET			:=	\033[0m

R_MARK_UP		:=	$(MAGENTA)$(BOLD)
CA_MARK_UP		:=	$(GREEN)$(BOLD)

# Current directory and formatted status messages
CUR_DIR			:=	$(dir $(abspath $(firstword $(MAKEFILE_LIST))))
REMOVED			:=	$(R_MARK_UP)REMOVED $(CYAN)%s$(MAGENTA) (%s) $(RESET)\n
CREATED			:=	$(CA_MARK_UP)CREATED $(CYAN)%s$(GREEN) (%s) $(RESET)\n
