NAME			:=	philo

MAKEFLAGS		+=	-j
COMPILER		:=	cc
COMPILER_INFO	:=	$(shell $(COMPILER) -v 2>&1)

COMPILER_KIND	:=	unknown
ifneq ($(findstring clang version,$(COMPILER_INFO)),)
COMPILER_KIND	:=	clang
endif
ifneq ($(findstring gcc version,$(COMPILER_INFO)),)
COMPILER_KIND	:=	gcc
endif

BASE_FLAGS		:=	-std=c99 -Wall -Wextra -Werror
THREAD_FLAGS	:=	-pthread

PEDANTIC		:=	-Wpedantic -pedantic-errors -Wundef -Wstrict-prototypes
WARNINGS		:=	-Wshadow -Wconversion -Wsign-conversion			\
					-Wformat=2 -Wuninitialized -Wunreachable-code

CAST_WARNINGS	:=	-Wbad-function-cast
ifeq ($(COMPILER_KIND),gcc)
CAST_WARNINGS	+=	-Wcast-function-type
endif

DEPFLAGS		:=	-MMD -MP

OPTIMIZATION	:=	-Ofast
SECURITY		:=	-fstack-protector-strong

ifeq ($(COMPILER_KIND),gcc)
OPTIMIZATION	+=	-flto=auto -fuse-linker-plugin
endif

ifeq ($(COMPILER_KIND),clang)
OPTIMIZATION	+=	-flto=thin
endif

ifeq ($(OS),Linux)
SECURITY		+=	-D_FORTIFY_SOURCE=2
FSANITIZE		:=	leak,
endif

SANITIZERS		:=	-fsanitize=$(FSANITIZE)address,undefined,null,integer-divide-by-zero,signed-integer-overflow,bounds,alignment
DEBUG_FLAGS		:=	-fno-omit-frame-pointer

CFLAGS			:=	$(BASE_FLAGS) $(THREAD_FLAGS) $(PEDANTIC) $(WARNINGS)	\
					$(CAST_WARNINGS) $(DEPFLAGS) $(OPTIMIZATION) $(SECURITY)

ifneq ($(filter valgrind,$(MAKECMDGOALS)),)
CFLAGS			+=	-g $(DEBUG_FLAGS)
else ifneq ($(filter debug,$(MAKECMDGOALS)),)
CFLAGS			+=	-g3 $(SANITIZERS) $(DEBUG_FLAGS) -fno-sanitize-recover=all
endif

CFLAGS			+=	$(if $(filter time,$(MAKECMDGOALS)),-D TIME=1000)
CFLAGS			+=	$(if $(filter readable,$(MAKECMDGOALS)),-D READABLE=true)
CFLAGS			+=	$(if $(filter no_rules,$(MAKECMDGOALS)),-D EXPLICIT_RULES=false)
CFLAGS			+=	$(if $(filter verbose,$(MAKECMDGOALS)),-D VERBOSE=true)
CFLAGS			+=	$(if $(filter valgrind_margin,$(MAKECMDGOALS)),-D VALGRIND_MARGIN=true)

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

# valgrind_margin --tool=helgrind --max-threads=10000 ./philo
time readable no_rules verbose valgrind_margin: all
	@:

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

.PHONY:	all												\
		time readable verbose no_rules valgrind_margin	\
		clean fclean re									\
		debug print-%

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
