#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=libRelease
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ui/uiFreeContainer.o \
	${OBJECTDIR}/MainWindow.o \
	${OBJECTDIR}/geo/Brush.o \
	${OBJECTDIR}/ui/stb_truetype.o \
	${OBJECTDIR}/ui/uiCheckbox.o \
	${OBJECTDIR}/ui/uiFont.o \
	${OBJECTDIR}/ui/uiVerticalContainer.o \
	${OBJECTDIR}/ui/uiRender3D.o \
	${OBJECTDIR}/common/vector3.o \
	${OBJECTDIR}/ui/uiValuebox.o \
	${OBJECTDIR}/geo/MapLoader.o \
	${OBJECTDIR}/geo/Entity.o \
	${OBJECTDIR}/ui/uiClipper.o \
	${OBJECTDIR}/ui/uiContainer.o \
	${OBJECTDIR}/geo/Scene.o \
	${OBJECTDIR}/ui/uiTextbox.o \
	${OBJECTDIR}/ui/uiUserInput.o \
	${OBJECTDIR}/ui/uiListbox.o \
	${OBJECTDIR}/ui/uiControl.o \
	${OBJECTDIR}/common/Tokenizer.o \
	${OBJECTDIR}/ui/uiManager.o \
	${OBJECTDIR}/ui/uiButton.o \
	${OBJECTDIR}/ui/uiLabel.o \
	${OBJECTDIR}/camera.o \
	${OBJECTDIR}/GlutApplication.o \
	${OBJECTDIR}/common/matrix4x4.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-libRelease.mk dist/libRelease/GNU-Linux-x86/libguiproject.a

dist/libRelease/GNU-Linux-x86/libguiproject.a: ${OBJECTFILES}
	${MKDIR} -p dist/libRelease/GNU-Linux-x86
	${RM} dist/libRelease/GNU-Linux-x86/libguiproject.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libguiproject.a ${OBJECTFILES} 
	$(RANLIB) dist/libRelease/GNU-Linux-x86/libguiproject.a

${OBJECTDIR}/ui/uiFreeContainer.o: ui/uiFreeContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiFreeContainer.o ui/uiFreeContainer.cpp

${OBJECTDIR}/MainWindow.o: MainWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/MainWindow.o MainWindow.cpp

${OBJECTDIR}/geo/Brush.o: geo/Brush.cpp 
	${MKDIR} -p ${OBJECTDIR}/geo
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/geo/Brush.o geo/Brush.cpp

${OBJECTDIR}/ui/stb_truetype.o: ui/stb_truetype.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/stb_truetype.o ui/stb_truetype.cpp

${OBJECTDIR}/ui/uiCheckbox.o: ui/uiCheckbox.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiCheckbox.o ui/uiCheckbox.cpp

${OBJECTDIR}/ui/uiFont.o: ui/uiFont.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiFont.o ui/uiFont.cpp

${OBJECTDIR}/ui/uiVerticalContainer.o: ui/uiVerticalContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiVerticalContainer.o ui/uiVerticalContainer.cpp

${OBJECTDIR}/ui/uiRender3D.o: ui/uiRender3D.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiRender3D.o ui/uiRender3D.cpp

${OBJECTDIR}/common/vector3.o: common/vector3.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/common/vector3.o common/vector3.cpp

${OBJECTDIR}/ui/uiValuebox.o: ui/uiValuebox.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiValuebox.o ui/uiValuebox.cpp

${OBJECTDIR}/geo/MapLoader.o: geo/MapLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/geo
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/geo/MapLoader.o geo/MapLoader.cpp

${OBJECTDIR}/geo/Entity.o: geo/Entity.cpp 
	${MKDIR} -p ${OBJECTDIR}/geo
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/geo/Entity.o geo/Entity.cpp

${OBJECTDIR}/ui/uiClipper.o: ui/uiClipper.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiClipper.o ui/uiClipper.cpp

${OBJECTDIR}/ui/uiContainer.o: ui/uiContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiContainer.o ui/uiContainer.cpp

${OBJECTDIR}/geo/Scene.o: geo/Scene.cpp 
	${MKDIR} -p ${OBJECTDIR}/geo
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/geo/Scene.o geo/Scene.cpp

${OBJECTDIR}/ui/uiTextbox.o: ui/uiTextbox.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiTextbox.o ui/uiTextbox.cpp

${OBJECTDIR}/ui/uiUserInput.o: ui/uiUserInput.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiUserInput.o ui/uiUserInput.cpp

${OBJECTDIR}/ui/uiListbox.o: ui/uiListbox.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiListbox.o ui/uiListbox.cpp

${OBJECTDIR}/ui/uiControl.o: ui/uiControl.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiControl.o ui/uiControl.cpp

${OBJECTDIR}/common/Tokenizer.o: common/Tokenizer.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/common/Tokenizer.o common/Tokenizer.cpp

${OBJECTDIR}/ui/uiManager.o: ui/uiManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiManager.o ui/uiManager.cpp

${OBJECTDIR}/ui/uiButton.o: ui/uiButton.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiButton.o ui/uiButton.cpp

${OBJECTDIR}/ui/uiLabel.o: ui/uiLabel.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiLabel.o ui/uiLabel.cpp

${OBJECTDIR}/camera.o: camera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/camera.o camera.cpp

${OBJECTDIR}/GlutApplication.o: GlutApplication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/GlutApplication.o GlutApplication.cpp

${OBJECTDIR}/common/matrix4x4.o: common/matrix4x4.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/common/matrix4x4.o common/matrix4x4.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/libRelease
	${RM} dist/libRelease/GNU-Linux-x86/libguiproject.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
