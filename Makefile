NAME			:=	philo

MAKEFLAGS		+=	-j
COMPILER		:=	cc

BASE_FLAGS		:=	-Wall -Wextra -Werror
# BASE_FLAGS		:=	-std=c99 -Wall -Wextra -Werror

# PEDANTIC		:=	-Wpedantic -pedantic-errors -Wundef -Wstrict-prototypes

# WARNINGS		:=	-Wshadow -Wconversion -Wsign-conversion			\
# 					-Wformat=2 -Wuninitialized -Wunreachable-code

# CAST_WARNINGS	:=	-Wbad-function-cast
# ifeq ($(shell $(COMPILER) --version | grep -c "gcc"),1)
# CAST_WARNINGS	+=	-Wcast-function-type
# endif

DEPFLAGS		:=	-MMD -MP

# OPTIMIZATION	:=	-O2
# SECURITY		:=	-fstack-protector-strong
# ifeq ($(shell uname -s),Linux)
# SECURITY		+=	-D_FORTIFY_SOURCE=2
# FSANITIZE		:=	leak
# endif

# SANITIZERS		:=	-fsanitize=$(FSANITIZE),address,undefined,null,integer-divide-by-zero,signed-integer-overflow,bounds,alignment
# DEBUG_FLAGS		:=	-fno-omit-frame-pointer

CFLAGS			:=	$(BASE_FLAGS) $(PEDANTIC) $(WARNINGS) $(CAST_WARNINGS) \
					$(DEPFLAGS) $(OPTIMIZATION) $(SECURITY)

CFLAGS			+=	-pthread

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

ifneq ($(filter helgrind,$(MAKECMDGOALS)),)
CFLAGS			+=	-g $(DEBUG_FLAGS)
else ifneq ($(filter debug,$(MAKECMDGOALS)),)
CFLAGS			+=	-g3 $(SANITIZERS) $(DEBUG_FLAGS) -fno-sanitize-recover=all
endif

PRINT_NO_DIR	:=	--no-print-directory
RM				:=	rm -rf

SRC_DIR			:=	src
INC_DIR			:=	include
BUILD_DIR		:=	.build

SRC				=	main.c						initialize.c					init_rules.c		\
					atoi_phil.c					init_pthreads_and_mutexes.c		time.c				\
					routine.c					activity.c						supervisor_thread.c	\
					print.c	clean_up.c			reporter_of_nodes_thread.c

SRC				:=	$(addprefix $(SRC_DIR)/, $(SRC))
OBJ				:=	$(SRC:%.c=$(BUILD_DIR)/%.o)
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
	$(COMPILER) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

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

# .PHONY: all			\
# 		clean fclean re	\
# 		debug helgrind print-%

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
