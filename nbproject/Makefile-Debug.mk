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
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/uiClipper.o \
	${OBJECTDIR}/MainWindow.o \
	${OBJECTDIR}/uiTextbox.o \
	${OBJECTDIR}/uiFreeContainer.o \
	${OBJECTDIR}/uiLabel.o \
	${OBJECTDIR}/uiContainer.o \
	${OBJECTDIR}/uiListbox.o \
	${OBJECTDIR}/uiFont.o \
	${OBJECTDIR}/stb_truetype.o \
	${OBJECTDIR}/uiUserInput.o \
	${OBJECTDIR}/uiButton.o \
	${OBJECTDIR}/uiCheckbox.o \
	${OBJECTDIR}/uiValuebox.o \
	${OBJECTDIR}/uiControl.o \
	${OBJECTDIR}/GlutApplication.o \
	${OBJECTDIR}/uiManager.o \
	${OBJECTDIR}/uiVerticalContainer.o


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
LDLIBSOPTIONS=-lglut -lGLU

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/guiproject

dist/Debug/GNU-Linux-x86/guiproject: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/guiproject ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/uiClipper.o: uiClipper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiClipper.o uiClipper.cpp

${OBJECTDIR}/MainWindow.o: MainWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/MainWindow.o MainWindow.cpp

${OBJECTDIR}/uiTextbox.o: uiTextbox.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiTextbox.o uiTextbox.cpp

${OBJECTDIR}/uiFreeContainer.o: uiFreeContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiFreeContainer.o uiFreeContainer.cpp

${OBJECTDIR}/uiLabel.o: uiLabel.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiLabel.o uiLabel.cpp

${OBJECTDIR}/uiContainer.o: uiContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiContainer.o uiContainer.cpp

${OBJECTDIR}/uiListbox.o: uiListbox.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiListbox.o uiListbox.cpp

${OBJECTDIR}/uiFont.o: uiFont.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiFont.o uiFont.cpp

${OBJECTDIR}/stb_truetype.o: stb_truetype.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/stb_truetype.o stb_truetype.cpp

${OBJECTDIR}/uiUserInput.o: uiUserInput.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiUserInput.o uiUserInput.cpp

${OBJECTDIR}/uiButton.o: uiButton.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiButton.o uiButton.cpp

${OBJECTDIR}/uiCheckbox.o: uiCheckbox.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiCheckbox.o uiCheckbox.cpp

${OBJECTDIR}/uiValuebox.o: uiValuebox.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiValuebox.o uiValuebox.cpp

${OBJECTDIR}/uiControl.o: uiControl.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiControl.o uiControl.cpp

${OBJECTDIR}/GlutApplication.o: GlutApplication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/GlutApplication.o GlutApplication.cpp

${OBJECTDIR}/uiManager.o: uiManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiManager.o uiManager.cpp

${OBJECTDIR}/uiVerticalContainer.o: uiVerticalContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/uiVerticalContainer.o uiVerticalContainer.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/guiproject

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
