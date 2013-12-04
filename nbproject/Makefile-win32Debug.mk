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
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=win32Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/GLee.o \
	${OBJECTDIR}/GlutApplication.o \
	${OBJECTDIR}/GuiProject.o \
	${OBJECTDIR}/Video.o \
	${OBJECTDIR}/common/FileLoader.o \
	${OBJECTDIR}/common/Texture.o \
	${OBJECTDIR}/common/TextureLoader.o \
	${OBJECTDIR}/common/Tokenizer.o \
	${OBJECTDIR}/common/camera.o \
	${OBJECTDIR}/common/matrix4x4.o \
	${OBJECTDIR}/common/quaternion.o \
	${OBJECTDIR}/common/vector3.o \
	${OBJECTDIR}/geo/Brush.o \
	${OBJECTDIR}/geo/Entity.o \
	${OBJECTDIR}/geo/MapLoader.o \
	${OBJECTDIR}/geo/Scene.o \
	${OBJECTDIR}/ui/stb_truetype.o \
	${OBJECTDIR}/ui/uiButton.o \
	${OBJECTDIR}/ui/uiCheckbox.o \
	${OBJECTDIR}/ui/uiClipper.o \
	${OBJECTDIR}/ui/uiContainer.o \
	${OBJECTDIR}/ui/uiControl.o \
	${OBJECTDIR}/ui/uiFont.o \
	${OBJECTDIR}/ui/uiFreeContainer.o \
	${OBJECTDIR}/ui/uiHorizontalContainer.o \
	${OBJECTDIR}/ui/uiLabel.o \
	${OBJECTDIR}/ui/uiListbox.o \
	${OBJECTDIR}/ui/uiManager.o \
	${OBJECTDIR}/ui/uiRender3D.o \
	${OBJECTDIR}/ui/uiTextbox.o \
	${OBJECTDIR}/ui/uiValuebox.o \
	${OBJECTDIR}/ui/uiVerticalContainer.o \
	${OBJECTDIR}/userInput.o


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
LDLIBSOPTIONS=-lfreeglut -lopengl32 -lglu32

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/guiproject.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/guiproject.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/guiproject ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/GLee.o: GLee.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/GLee.o GLee.c

${OBJECTDIR}/GlutApplication.o: GlutApplication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/GlutApplication.o GlutApplication.cpp

${OBJECTDIR}/GuiProject.o: GuiProject.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/GuiProject.o GuiProject.cpp

${OBJECTDIR}/Video.o: Video.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/Video.o Video.cpp

${OBJECTDIR}/common/FileLoader.o: common/FileLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/common/FileLoader.o common/FileLoader.cpp

${OBJECTDIR}/common/Texture.o: common/Texture.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/common/Texture.o common/Texture.cpp

${OBJECTDIR}/common/TextureLoader.o: common/TextureLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/common/TextureLoader.o common/TextureLoader.cpp

${OBJECTDIR}/common/Tokenizer.o: common/Tokenizer.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/common/Tokenizer.o common/Tokenizer.cpp

${OBJECTDIR}/common/camera.o: common/camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/common/camera.o common/camera.cpp

${OBJECTDIR}/common/matrix4x4.o: common/matrix4x4.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/common/matrix4x4.o common/matrix4x4.cpp

${OBJECTDIR}/common/quaternion.o: common/quaternion.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/common/quaternion.o common/quaternion.cpp

${OBJECTDIR}/common/vector3.o: common/vector3.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/common/vector3.o common/vector3.cpp

${OBJECTDIR}/geo/Brush.o: geo/Brush.cpp 
	${MKDIR} -p ${OBJECTDIR}/geo
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/geo/Brush.o geo/Brush.cpp

${OBJECTDIR}/geo/Entity.o: geo/Entity.cpp 
	${MKDIR} -p ${OBJECTDIR}/geo
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/geo/Entity.o geo/Entity.cpp

${OBJECTDIR}/geo/MapLoader.o: geo/MapLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/geo
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/geo/MapLoader.o geo/MapLoader.cpp

${OBJECTDIR}/geo/Scene.o: geo/Scene.cpp 
	${MKDIR} -p ${OBJECTDIR}/geo
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/geo/Scene.o geo/Scene.cpp

${OBJECTDIR}/ui/stb_truetype.o: ui/stb_truetype.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/stb_truetype.o ui/stb_truetype.cpp

${OBJECTDIR}/ui/uiButton.o: ui/uiButton.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiButton.o ui/uiButton.cpp

${OBJECTDIR}/ui/uiCheckbox.o: ui/uiCheckbox.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiCheckbox.o ui/uiCheckbox.cpp

${OBJECTDIR}/ui/uiClipper.o: ui/uiClipper.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiClipper.o ui/uiClipper.cpp

${OBJECTDIR}/ui/uiContainer.o: ui/uiContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiContainer.o ui/uiContainer.cpp

${OBJECTDIR}/ui/uiControl.o: ui/uiControl.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiControl.o ui/uiControl.cpp

${OBJECTDIR}/ui/uiFont.o: ui/uiFont.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiFont.o ui/uiFont.cpp

${OBJECTDIR}/ui/uiFreeContainer.o: ui/uiFreeContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiFreeContainer.o ui/uiFreeContainer.cpp

${OBJECTDIR}/ui/uiHorizontalContainer.o: ui/uiHorizontalContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiHorizontalContainer.o ui/uiHorizontalContainer.cpp

${OBJECTDIR}/ui/uiLabel.o: ui/uiLabel.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiLabel.o ui/uiLabel.cpp

${OBJECTDIR}/ui/uiListbox.o: ui/uiListbox.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiListbox.o ui/uiListbox.cpp

${OBJECTDIR}/ui/uiManager.o: ui/uiManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiManager.o ui/uiManager.cpp

${OBJECTDIR}/ui/uiRender3D.o: ui/uiRender3D.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiRender3D.o ui/uiRender3D.cpp

${OBJECTDIR}/ui/uiTextbox.o: ui/uiTextbox.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiTextbox.o ui/uiTextbox.cpp

${OBJECTDIR}/ui/uiValuebox.o: ui/uiValuebox.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiValuebox.o ui/uiValuebox.cpp

${OBJECTDIR}/ui/uiVerticalContainer.o: ui/uiVerticalContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/ui
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ui/uiVerticalContainer.o ui/uiVerticalContainer.cpp

${OBJECTDIR}/userInput.o: userInput.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/userInput.o userInput.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/guiproject.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
