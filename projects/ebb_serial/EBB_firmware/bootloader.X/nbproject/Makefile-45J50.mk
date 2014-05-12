#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-45J50.mk)" "nbproject/Makefile-local-45J50.mk"
include nbproject/Makefile-local-45J50.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=45J50
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/source/Boot46J50Family.o ${OBJECTDIR}/source/hid.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/usb9.o ${OBJECTDIR}/source/usbctrltrf.o ${OBJECTDIR}/source/usbdrv.o ${OBJECTDIR}/source/usbdsc.o ${OBJECTDIR}/source/usbmmap.o
POSSIBLE_DEPFILES=${OBJECTDIR}/source/Boot46J50Family.o.d ${OBJECTDIR}/source/hid.o.d ${OBJECTDIR}/source/main.o.d ${OBJECTDIR}/source/usb9.o.d ${OBJECTDIR}/source/usbctrltrf.o.d ${OBJECTDIR}/source/usbdrv.o.d ${OBJECTDIR}/source/usbdsc.o.d ${OBJECTDIR}/source/usbmmap.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/source/Boot46J50Family.o ${OBJECTDIR}/source/hid.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/usb9.o ${OBJECTDIR}/source/usbctrltrf.o ${OBJECTDIR}/source/usbdrv.o ${OBJECTDIR}/source/usbdsc.o ${OBJECTDIR}/source/usbmmap.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-45J50.mk dist/${CND_CONF}/${IMAGE_TYPE}/bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F45J50
MP_PROCESSOR_OPTION_LD=18f45j50
MP_LINKER_DEBUG_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/source/Boot46J50Family.o: source/Boot46J50Family.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/Boot46J50Family.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/Boot46J50Family.o   source/Boot46J50Family.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/Boot46J50Family.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Boot46J50Family.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/hid.o: source/hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/hid.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/hid.o   source/hid.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/hid.o 
	@${FIXDEPS} "${OBJECTDIR}/source/hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/main.o   source/main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/usb9.o: source/usb9.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/usb9.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/usb9.o   source/usb9.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/usb9.o 
	@${FIXDEPS} "${OBJECTDIR}/source/usb9.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/usbctrltrf.o: source/usbctrltrf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/usbctrltrf.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/usbctrltrf.o   source/usbctrltrf.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/usbctrltrf.o 
	@${FIXDEPS} "${OBJECTDIR}/source/usbctrltrf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/usbdrv.o: source/usbdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/usbdrv.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/usbdrv.o   source/usbdrv.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/usbdrv.o 
	@${FIXDEPS} "${OBJECTDIR}/source/usbdrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/usbdsc.o: source/usbdsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/usbdsc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/usbdsc.o   source/usbdsc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/usbdsc.o 
	@${FIXDEPS} "${OBJECTDIR}/source/usbdsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/usbmmap.o: source/usbmmap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/usbmmap.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/usbmmap.o   source/usbmmap.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/usbmmap.o 
	@${FIXDEPS} "${OBJECTDIR}/source/usbmmap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/source/Boot46J50Family.o: source/Boot46J50Family.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/Boot46J50Family.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/Boot46J50Family.o   source/Boot46J50Family.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/Boot46J50Family.o 
	@${FIXDEPS} "${OBJECTDIR}/source/Boot46J50Family.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/hid.o: source/hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/hid.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/hid.o   source/hid.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/hid.o 
	@${FIXDEPS} "${OBJECTDIR}/source/hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/main.o   source/main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/usb9.o: source/usb9.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/usb9.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/usb9.o   source/usb9.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/usb9.o 
	@${FIXDEPS} "${OBJECTDIR}/source/usb9.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/usbctrltrf.o: source/usbctrltrf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/usbctrltrf.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/usbctrltrf.o   source/usbctrltrf.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/usbctrltrf.o 
	@${FIXDEPS} "${OBJECTDIR}/source/usbctrltrf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/usbdrv.o: source/usbdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/usbdrv.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/usbdrv.o   source/usbdrv.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/usbdrv.o 
	@${FIXDEPS} "${OBJECTDIR}/source/usbdrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/usbdsc.o: source/usbdsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/usbdsc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/usbdsc.o   source/usbdsc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/usbdsc.o 
	@${FIXDEPS} "${OBJECTDIR}/source/usbdsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/source/usbmmap.o: source/usbmmap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/usbmmap.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -DEBB_V11 -I"." -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/source/usbmmap.o   source/usbmmap.c 
	@${DEP_GEN} -d ${OBJECTDIR}/source/usbmmap.o 
	@${FIXDEPS} "${OBJECTDIR}/source/usbmmap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    source/BootModified.rm18f45j50_g.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "source\BootModified.rm18f45j50_g.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/bootlaoder.X.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_ICD3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   source/BootModified.rm18f45j50_g.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "source\BootModified.rm18f45j50_g.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/bootlaoder.X.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/bootloader.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/45J50
	${RM} -r dist/45J50

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
