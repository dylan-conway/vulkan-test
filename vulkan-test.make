# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
INCLUDES += -I"$(VK_SDK_PATH)/Include" -Iinclude
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS += -lmingw32 -lSDL2main -lSDL2 -lvulkan-1 -lshaderc_shared
LDDEPS +=
LINKCMD = $(CC) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
CC=gcc

define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug)
TARGETDIR = build/Debug
TARGET = $(TARGETDIR)/vulkan-test.exe
OBJDIR = obj/Debug
DEFINES += -DDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -g -Wall -Wextra -Wno-unused-parameter -Wno-unused-function
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -g -Wall -Wextra -Wno-unused-parameter -Wno-unused-function
ALL_LDFLAGS += $(LDFLAGS) -L"$(VK_SDK_PATH)/Lib"

else ifeq ($(config),release)
TARGETDIR = build/Release
TARGET = $(TARGETDIR)/vulkan-test.exe
OBJDIR = obj/Release
DEFINES +=
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O2
ALL_LDFLAGS += $(LDFLAGS) -L"$(VK_SDK_PATH)/Lib" -s

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/Buffer.o
GENERATED += $(OBJDIR)/CommandPool.o
GENERATED += $(OBJDIR)/Device.o
GENERATED += $(OBJDIR)/Fence.o
GENERATED += $(OBJDIR)/Instance.o
GENERATED += $(OBJDIR)/PhysicalDevice.o
GENERATED += $(OBJDIR)/Pipeline.o
GENERATED += $(OBJDIR)/Semaphore.o
GENERATED += $(OBJDIR)/Shader.o
GENERATED += $(OBJDIR)/Surface.o
GENERATED += $(OBJDIR)/Swapchain.o
GENERATED += $(OBJDIR)/cwindow.o
GENERATED += $(OBJDIR)/cwindow_renderer.o
GENERATED += $(OBJDIR)/dc_utils.o
GENERATED += $(OBJDIR)/dvec.o
GENERATED += $(OBJDIR)/gvec.o
GENERATED += $(OBJDIR)/main.o
GENERATED += $(OBJDIR)/obj.o
GENERATED += $(OBJDIR)/vec.o
OBJECTS += $(OBJDIR)/Buffer.o
OBJECTS += $(OBJDIR)/CommandPool.o
OBJECTS += $(OBJDIR)/Device.o
OBJECTS += $(OBJDIR)/Fence.o
OBJECTS += $(OBJDIR)/Instance.o
OBJECTS += $(OBJDIR)/PhysicalDevice.o
OBJECTS += $(OBJDIR)/Pipeline.o
OBJECTS += $(OBJDIR)/Semaphore.o
OBJECTS += $(OBJDIR)/Shader.o
OBJECTS += $(OBJDIR)/Surface.o
OBJECTS += $(OBJDIR)/Swapchain.o
OBJECTS += $(OBJDIR)/cwindow.o
OBJECTS += $(OBJDIR)/cwindow_renderer.o
OBJECTS += $(OBJDIR)/dc_utils.o
OBJECTS += $(OBJDIR)/dvec.o
OBJECTS += $(OBJDIR)/gvec.o
OBJECTS += $(OBJDIR)/main.o
OBJECTS += $(OBJDIR)/obj.o
OBJECTS += $(OBJDIR)/vec.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking vulkan-test
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning vulkan-test
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) rmdir /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CC) -x c-header $(ALL_CFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/cwindow.o: src/cwindow/cwindow.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cwindow_renderer.o: src/cwindow/cwindow_renderer.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Buffer.o: src/cwindow/cwindow_renderer/Buffer.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/CommandPool.o: src/cwindow/cwindow_renderer/CommandPool.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Device.o: src/cwindow/cwindow_renderer/Device.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Fence.o: src/cwindow/cwindow_renderer/Fence.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Instance.o: src/cwindow/cwindow_renderer/Instance.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PhysicalDevice.o: src/cwindow/cwindow_renderer/PhysicalDevice.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Pipeline.o: src/cwindow/cwindow_renderer/Pipeline.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Semaphore.o: src/cwindow/cwindow_renderer/Semaphore.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Shader.o: src/cwindow/cwindow_renderer/Shader.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Surface.o: src/cwindow/cwindow_renderer/Surface.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Swapchain.o: src/cwindow/cwindow_renderer/Swapchain.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/dc_utils.o: src/dc_utils.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/dvec.o: src/dvec.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/gvec.o: src/gvec.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/main.o: src/main.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/obj.o: src/obj.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/vec.o: src/vec.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif