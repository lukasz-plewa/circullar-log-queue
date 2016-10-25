RM := rm -rf
MKDIR := mkdir -vp

OUTDIR := build
# the build target executable:
TARGET = circular_log

C_SRCS += \
	  src/circular_log.c

OBJS += \
	$(C_SRCS:src/%.c=$(OUTDIR)/%.o)

C_DEPS += \
	  ./$(OUTDIR)/circular_log.d 

# compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(TARGET)

# Tool invocations
$(TARGET): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	@echo 'OBJS=$(OBJS)'
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)
	mv $(TARGET) $(OUTDIR)/$(TARGET)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	@echo 'OBJS=$(OBJS)'
	-$(RM) $(OBJS) $(C_DEPS) $(EXECUTABLES) $(OUTDIR)
	-@echo ' '

$(OUTDIR)/%.o: src/%.c
	$(MKDIR) build
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo 'Finished building: $<'
	@echo ' '

.PHONY: all clean dependents
.SECONDARY:

