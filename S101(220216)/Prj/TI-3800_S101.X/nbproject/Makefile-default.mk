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
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/TI-3800_S101.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/TI-3800_S101.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED="../../Flash Operations.s" ../../User_Source/adc.c ../../User_Source/display.c ../../User_Source/eepr.c ../../User_Source/ex_var.c ../../User_Source/flash.c ../../User_Source/interrupt.c ../../User_Source/key.c ../../User_Source/main.c ../../User_Source/sub_function.c ../../User_Source/test.c ../../User_Source/uart.c ../../User_Source/rtc.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED="${OBJECTDIR}/_ext/43898991/Flash Operations.o" ${OBJECTDIR}/_ext/2069935761/adc.o ${OBJECTDIR}/_ext/2069935761/display.o ${OBJECTDIR}/_ext/2069935761/eepr.o ${OBJECTDIR}/_ext/2069935761/ex_var.o ${OBJECTDIR}/_ext/2069935761/flash.o ${OBJECTDIR}/_ext/2069935761/interrupt.o ${OBJECTDIR}/_ext/2069935761/key.o ${OBJECTDIR}/_ext/2069935761/main.o ${OBJECTDIR}/_ext/2069935761/sub_function.o ${OBJECTDIR}/_ext/2069935761/test.o ${OBJECTDIR}/_ext/2069935761/uart.o ${OBJECTDIR}/_ext/2069935761/rtc.o
POSSIBLE_DEPFILES="${OBJECTDIR}/_ext/43898991/Flash Operations.o.d" ${OBJECTDIR}/_ext/2069935761/adc.o.d ${OBJECTDIR}/_ext/2069935761/display.o.d ${OBJECTDIR}/_ext/2069935761/eepr.o.d ${OBJECTDIR}/_ext/2069935761/ex_var.o.d ${OBJECTDIR}/_ext/2069935761/flash.o.d ${OBJECTDIR}/_ext/2069935761/interrupt.o.d ${OBJECTDIR}/_ext/2069935761/key.o.d ${OBJECTDIR}/_ext/2069935761/main.o.d ${OBJECTDIR}/_ext/2069935761/sub_function.o.d ${OBJECTDIR}/_ext/2069935761/test.o.d ${OBJECTDIR}/_ext/2069935761/uart.o.d ${OBJECTDIR}/_ext/2069935761/rtc.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/43898991/Flash\ Operations.o ${OBJECTDIR}/_ext/2069935761/adc.o ${OBJECTDIR}/_ext/2069935761/display.o ${OBJECTDIR}/_ext/2069935761/eepr.o ${OBJECTDIR}/_ext/2069935761/ex_var.o ${OBJECTDIR}/_ext/2069935761/flash.o ${OBJECTDIR}/_ext/2069935761/interrupt.o ${OBJECTDIR}/_ext/2069935761/key.o ${OBJECTDIR}/_ext/2069935761/main.o ${OBJECTDIR}/_ext/2069935761/sub_function.o ${OBJECTDIR}/_ext/2069935761/test.o ${OBJECTDIR}/_ext/2069935761/uart.o ${OBJECTDIR}/_ext/2069935761/rtc.o

# Source Files
SOURCEFILES=../../Flash Operations.s ../../User_Source/adc.c ../../User_Source/display.c ../../User_Source/eepr.c ../../User_Source/ex_var.c ../../User_Source/flash.c ../../User_Source/interrupt.c ../../User_Source/key.c ../../User_Source/main.c ../../User_Source/sub_function.c ../../User_Source/test.c ../../User_Source/uart.c ../../User_Source/rtc.c


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
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/TI-3800_S101.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ64GA306
MP_LINKER_FILE_OPTION=,-Tp24FJ64GA306.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/43898991/Flash\ Operations.o: ../../Flash\ Operations.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} "${OBJECTDIR}/_ext/43898991/Flash Operations.o".d 
	@${RM} "${OBJECTDIR}/_ext/43898991/Flash Operations.o".ok ${OBJECTDIR}/_ext/43898991/Flash\ Operations.o.err 
	@${RM} "${OBJECTDIR}/_ext/43898991/Flash Operations.o" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/Flash Operations.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_AS} $(MP_EXTRA_AS_PRE)  "../../Flash Operations.s" -o "${OBJECTDIR}/_ext/43898991/Flash Operations.o" -omf=elf -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 --defsym=__MPLAB_DEBUG=1 --defsym=__ICD2RAM=1 --defsym=__DEBUG=1 --defsym=__MPLAB_DEBUGGER_PK3=1 -g  -MD "${OBJECTDIR}/_ext/43898991/Flash Operations.o.d"$(MP_EXTRA_AS_POST)
	
else
${OBJECTDIR}/_ext/43898991/Flash\ Operations.o: ../../Flash\ Operations.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} "${OBJECTDIR}/_ext/43898991/Flash Operations.o".d 
	@${RM} "${OBJECTDIR}/_ext/43898991/Flash Operations.o".ok ${OBJECTDIR}/_ext/43898991/Flash\ Operations.o.err 
	@${RM} "${OBJECTDIR}/_ext/43898991/Flash Operations.o" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/Flash Operations.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_AS} $(MP_EXTRA_AS_PRE)  "../../Flash Operations.s" -o "${OBJECTDIR}/_ext/43898991/Flash Operations.o" -omf=elf -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 -g  -MD "${OBJECTDIR}/_ext/43898991/Flash Operations.o.d"$(MP_EXTRA_AS_POST)
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/2069935761/adc.o: ../../User_Source/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/adc.o.ok ${OBJECTDIR}/_ext/2069935761/adc.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/adc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/adc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/adc.o.d" -o ${OBJECTDIR}/_ext/2069935761/adc.o ../../User_Source/adc.c    
	
${OBJECTDIR}/_ext/2069935761/display.o: ../../User_Source/display.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/display.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/display.o.ok ${OBJECTDIR}/_ext/2069935761/display.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/display.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/display.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/display.o.d" -o ${OBJECTDIR}/_ext/2069935761/display.o ../../User_Source/display.c    
	
${OBJECTDIR}/_ext/2069935761/eepr.o: ../../User_Source/eepr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/eepr.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/eepr.o.ok ${OBJECTDIR}/_ext/2069935761/eepr.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/eepr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/eepr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/eepr.o.d" -o ${OBJECTDIR}/_ext/2069935761/eepr.o ../../User_Source/eepr.c    
	
${OBJECTDIR}/_ext/2069935761/ex_var.o: ../../User_Source/ex_var.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/ex_var.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/ex_var.o.ok ${OBJECTDIR}/_ext/2069935761/ex_var.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/ex_var.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/ex_var.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/ex_var.o.d" -o ${OBJECTDIR}/_ext/2069935761/ex_var.o ../../User_Source/ex_var.c    
	
${OBJECTDIR}/_ext/2069935761/flash.o: ../../User_Source/flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/flash.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/flash.o.ok ${OBJECTDIR}/_ext/2069935761/flash.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/flash.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/flash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/flash.o.d" -o ${OBJECTDIR}/_ext/2069935761/flash.o ../../User_Source/flash.c    
	
${OBJECTDIR}/_ext/2069935761/interrupt.o: ../../User_Source/interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/interrupt.o.ok ${OBJECTDIR}/_ext/2069935761/interrupt.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/interrupt.o.d" -o ${OBJECTDIR}/_ext/2069935761/interrupt.o ../../User_Source/interrupt.c    
	
${OBJECTDIR}/_ext/2069935761/key.o: ../../User_Source/key.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/key.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/key.o.ok ${OBJECTDIR}/_ext/2069935761/key.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/key.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/key.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/key.o.d" -o ${OBJECTDIR}/_ext/2069935761/key.o ../../User_Source/key.c    
	
${OBJECTDIR}/_ext/2069935761/main.o: ../../User_Source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/main.o.ok ${OBJECTDIR}/_ext/2069935761/main.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/main.o.d" -o ${OBJECTDIR}/_ext/2069935761/main.o ../../User_Source/main.c    
	
${OBJECTDIR}/_ext/2069935761/sub_function.o: ../../User_Source/sub_function.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/sub_function.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/sub_function.o.ok ${OBJECTDIR}/_ext/2069935761/sub_function.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/sub_function.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/sub_function.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/sub_function.o.d" -o ${OBJECTDIR}/_ext/2069935761/sub_function.o ../../User_Source/sub_function.c    
	
${OBJECTDIR}/_ext/2069935761/test.o: ../../User_Source/test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/test.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/test.o.ok ${OBJECTDIR}/_ext/2069935761/test.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/test.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/test.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/test.o.d" -o ${OBJECTDIR}/_ext/2069935761/test.o ../../User_Source/test.c    
	
${OBJECTDIR}/_ext/2069935761/uart.o: ../../User_Source/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/uart.o.ok ${OBJECTDIR}/_ext/2069935761/uart.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/uart.o.d" -o ${OBJECTDIR}/_ext/2069935761/uart.o ../../User_Source/uart.c    
	
${OBJECTDIR}/_ext/2069935761/rtc.o: ../../User_Source/rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/rtc.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/rtc.o.ok ${OBJECTDIR}/_ext/2069935761/rtc.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/rtc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/rtc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/rtc.o.d" -o ${OBJECTDIR}/_ext/2069935761/rtc.o ../../User_Source/rtc.c    
	
else
${OBJECTDIR}/_ext/2069935761/adc.o: ../../User_Source/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/adc.o.ok ${OBJECTDIR}/_ext/2069935761/adc.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/adc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/adc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/adc.o.d" -o ${OBJECTDIR}/_ext/2069935761/adc.o ../../User_Source/adc.c    
	
${OBJECTDIR}/_ext/2069935761/display.o: ../../User_Source/display.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/display.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/display.o.ok ${OBJECTDIR}/_ext/2069935761/display.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/display.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/display.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/display.o.d" -o ${OBJECTDIR}/_ext/2069935761/display.o ../../User_Source/display.c    
	
${OBJECTDIR}/_ext/2069935761/eepr.o: ../../User_Source/eepr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/eepr.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/eepr.o.ok ${OBJECTDIR}/_ext/2069935761/eepr.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/eepr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/eepr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/eepr.o.d" -o ${OBJECTDIR}/_ext/2069935761/eepr.o ../../User_Source/eepr.c    
	
${OBJECTDIR}/_ext/2069935761/ex_var.o: ../../User_Source/ex_var.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/ex_var.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/ex_var.o.ok ${OBJECTDIR}/_ext/2069935761/ex_var.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/ex_var.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/ex_var.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/ex_var.o.d" -o ${OBJECTDIR}/_ext/2069935761/ex_var.o ../../User_Source/ex_var.c    
	
${OBJECTDIR}/_ext/2069935761/flash.o: ../../User_Source/flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/flash.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/flash.o.ok ${OBJECTDIR}/_ext/2069935761/flash.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/flash.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/flash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/flash.o.d" -o ${OBJECTDIR}/_ext/2069935761/flash.o ../../User_Source/flash.c    
	
${OBJECTDIR}/_ext/2069935761/interrupt.o: ../../User_Source/interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/interrupt.o.ok ${OBJECTDIR}/_ext/2069935761/interrupt.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/interrupt.o.d" -o ${OBJECTDIR}/_ext/2069935761/interrupt.o ../../User_Source/interrupt.c    
	
${OBJECTDIR}/_ext/2069935761/key.o: ../../User_Source/key.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/key.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/key.o.ok ${OBJECTDIR}/_ext/2069935761/key.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/key.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/key.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/key.o.d" -o ${OBJECTDIR}/_ext/2069935761/key.o ../../User_Source/key.c    
	
${OBJECTDIR}/_ext/2069935761/main.o: ../../User_Source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/main.o.ok ${OBJECTDIR}/_ext/2069935761/main.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/main.o.d" -o ${OBJECTDIR}/_ext/2069935761/main.o ../../User_Source/main.c    
	
${OBJECTDIR}/_ext/2069935761/sub_function.o: ../../User_Source/sub_function.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/sub_function.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/sub_function.o.ok ${OBJECTDIR}/_ext/2069935761/sub_function.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/sub_function.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/sub_function.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/sub_function.o.d" -o ${OBJECTDIR}/_ext/2069935761/sub_function.o ../../User_Source/sub_function.c    
	
${OBJECTDIR}/_ext/2069935761/test.o: ../../User_Source/test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/test.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/test.o.ok ${OBJECTDIR}/_ext/2069935761/test.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/test.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/test.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/test.o.d" -o ${OBJECTDIR}/_ext/2069935761/test.o ../../User_Source/test.c    
	
${OBJECTDIR}/_ext/2069935761/uart.o: ../../User_Source/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/uart.o.ok ${OBJECTDIR}/_ext/2069935761/uart.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/uart.o.d" -o ${OBJECTDIR}/_ext/2069935761/uart.o ../../User_Source/uart.c    
	
${OBJECTDIR}/_ext/2069935761/rtc.o: ../../User_Source/rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2069935761" 
	@${RM} ${OBJECTDIR}/_ext/2069935761/rtc.o.d 
	@${RM} ${OBJECTDIR}/_ext/2069935761/rtc.o.ok ${OBJECTDIR}/_ext/2069935761/rtc.o.err 
	@${RM} ${OBJECTDIR}/_ext/2069935761/rtc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2069935761/rtc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Os -MMD -MF "${OBJECTDIR}/_ext/2069935761/rtc.o.d" -o ${OBJECTDIR}/_ext/2069935761/rtc.o ../../User_Source/rtc.c    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/TI-3800_S101.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -o dist/${CND_CONF}/${IMAGE_TYPE}/TI-3800_S101.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/TI-3800_S101.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/TI-3800_S101.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/TI-3800_S101.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=elf
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
