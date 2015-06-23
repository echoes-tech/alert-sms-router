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
CCC=DISTCC_FALLBACK=0 distcc g++-4.7
CXX=DISTCC_FALLBACK=0 distcc g++-4.7
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AckResource.o \
	${OBJECTDIR}/AswResource.o \
	${OBJECTDIR}/Conf.o \
	${OBJECTDIR}/PublicApiResource.o \
	${OBJECTDIR}/SavedSend.o \
	${OBJECTDIR}/SendResource.o \
	${OBJECTDIR}/Session.o \
	${OBJECTDIR}/StateMessage.o \
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=-lboost_program_options -lwt -lwthttp -lwtdbo -lwtdbosqlite3 -lwtdbopostgres

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sms-routeur

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sms-routeur: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sms-routeur ${OBJECTFILES} ${LDLIBSOPTIONS} -lboost_system

${OBJECTDIR}/AckResource.o: AckResource.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AckResource.o AckResource.cpp

${OBJECTDIR}/AswResource.o: AswResource.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AswResource.o AswResource.cpp

${OBJECTDIR}/Conf.o: Conf.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Conf.o Conf.cpp

${OBJECTDIR}/PublicApiResource.o: PublicApiResource.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PublicApiResource.o PublicApiResource.cpp

${OBJECTDIR}/SavedSend.o: SavedSend.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SavedSend.o SavedSend.cpp

${OBJECTDIR}/SendResource.o: SendResource.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SendResource.o SendResource.cpp

${OBJECTDIR}/Session.o: Session.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Session.o Session.cpp

${OBJECTDIR}/StateMessage.o: StateMessage.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/StateMessage.o StateMessage.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sms-routeur

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
