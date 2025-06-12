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
ifeq "$(wildcard nbproject/Makefile-local-PIC18F26K80.mk)" "nbproject/Makefile-local-PIC18F26K80.mk"
include nbproject/Makefile-local-PIC18F26K80.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PIC18F26K80
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Universal-VLCB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Universal-VLCB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../main.c ../digitalOut.c ../inputs.c ../event_consumerDualActionQueue.c ../universalEvents.c ../universalNv.c ../outputs.c ../servo.c ../bounce.c ../analogue.c ../../VLCBlib_PIC/boot.c ../../VLCBlib_PIC/event_coe.c ../../VLCBlib_PIC/mns.c ../../VLCBlib_PIC/nv.c ../../VLCBlib_PIC/statusLeds2.c ../../VLCBlib_PIC/ticktime.c ../../VLCBlib_PIC/timedResponse.c ../../VLCBlib_PIC/vlcb.c ../../VLCBlib_PIC/messageQueue.c ../../VLCBlib_PIC/nvm.c ../../VLCBlib_PIC/can18_ecan.c ../../VLCBlib_PIC/event_teach_large.c ../../VLCBlib_PIC/event_producer_happening.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/main.p1 ${OBJECTDIR}/_ext/1472/digitalOut.p1 ${OBJECTDIR}/_ext/1472/inputs.p1 ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1 ${OBJECTDIR}/_ext/1472/universalEvents.p1 ${OBJECTDIR}/_ext/1472/universalNv.p1 ${OBJECTDIR}/_ext/1472/outputs.p1 ${OBJECTDIR}/_ext/1472/servo.p1 ${OBJECTDIR}/_ext/1472/bounce.p1 ${OBJECTDIR}/_ext/1472/analogue.p1 ${OBJECTDIR}/_ext/1954642981/boot.p1 ${OBJECTDIR}/_ext/1954642981/event_coe.p1 ${OBJECTDIR}/_ext/1954642981/mns.p1 ${OBJECTDIR}/_ext/1954642981/nv.p1 ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1 ${OBJECTDIR}/_ext/1954642981/ticktime.p1 ${OBJECTDIR}/_ext/1954642981/timedResponse.p1 ${OBJECTDIR}/_ext/1954642981/vlcb.p1 ${OBJECTDIR}/_ext/1954642981/messageQueue.p1 ${OBJECTDIR}/_ext/1954642981/nvm.p1 ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1 ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1 ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/main.p1.d ${OBJECTDIR}/_ext/1472/digitalOut.p1.d ${OBJECTDIR}/_ext/1472/inputs.p1.d ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1.d ${OBJECTDIR}/_ext/1472/universalEvents.p1.d ${OBJECTDIR}/_ext/1472/universalNv.p1.d ${OBJECTDIR}/_ext/1472/outputs.p1.d ${OBJECTDIR}/_ext/1472/servo.p1.d ${OBJECTDIR}/_ext/1472/bounce.p1.d ${OBJECTDIR}/_ext/1472/analogue.p1.d ${OBJECTDIR}/_ext/1954642981/boot.p1.d ${OBJECTDIR}/_ext/1954642981/event_coe.p1.d ${OBJECTDIR}/_ext/1954642981/mns.p1.d ${OBJECTDIR}/_ext/1954642981/nv.p1.d ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1.d ${OBJECTDIR}/_ext/1954642981/ticktime.p1.d ${OBJECTDIR}/_ext/1954642981/timedResponse.p1.d ${OBJECTDIR}/_ext/1954642981/vlcb.p1.d ${OBJECTDIR}/_ext/1954642981/messageQueue.p1.d ${OBJECTDIR}/_ext/1954642981/nvm.p1.d ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1.d ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1.d ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/main.p1 ${OBJECTDIR}/_ext/1472/digitalOut.p1 ${OBJECTDIR}/_ext/1472/inputs.p1 ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1 ${OBJECTDIR}/_ext/1472/universalEvents.p1 ${OBJECTDIR}/_ext/1472/universalNv.p1 ${OBJECTDIR}/_ext/1472/outputs.p1 ${OBJECTDIR}/_ext/1472/servo.p1 ${OBJECTDIR}/_ext/1472/bounce.p1 ${OBJECTDIR}/_ext/1472/analogue.p1 ${OBJECTDIR}/_ext/1954642981/boot.p1 ${OBJECTDIR}/_ext/1954642981/event_coe.p1 ${OBJECTDIR}/_ext/1954642981/mns.p1 ${OBJECTDIR}/_ext/1954642981/nv.p1 ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1 ${OBJECTDIR}/_ext/1954642981/ticktime.p1 ${OBJECTDIR}/_ext/1954642981/timedResponse.p1 ${OBJECTDIR}/_ext/1954642981/vlcb.p1 ${OBJECTDIR}/_ext/1954642981/messageQueue.p1 ${OBJECTDIR}/_ext/1954642981/nvm.p1 ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1 ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1 ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1

# Source Files
SOURCEFILES=../main.c ../digitalOut.c ../inputs.c ../event_consumerDualActionQueue.c ../universalEvents.c ../universalNv.c ../outputs.c ../servo.c ../bounce.c ../analogue.c ../../VLCBlib_PIC/boot.c ../../VLCBlib_PIC/event_coe.c ../../VLCBlib_PIC/mns.c ../../VLCBlib_PIC/nv.c ../../VLCBlib_PIC/statusLeds2.c ../../VLCBlib_PIC/ticktime.c ../../VLCBlib_PIC/timedResponse.c ../../VLCBlib_PIC/vlcb.c ../../VLCBlib_PIC/messageQueue.c ../../VLCBlib_PIC/nvm.c ../../VLCBlib_PIC/can18_ecan.c ../../VLCBlib_PIC/event_teach_large.c ../../VLCBlib_PIC/event_producer_happening.c



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
	${MAKE}  -f nbproject/Makefile-PIC18F26K80.mk ${DISTDIR}/Universal-VLCB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F26K80
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/main.p1: ../main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/main.p1 ../main.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/main.d ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/digitalOut.p1: ../digitalOut.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/digitalOut.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/digitalOut.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/digitalOut.p1 ../digitalOut.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/digitalOut.d ${OBJECTDIR}/_ext/1472/digitalOut.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/digitalOut.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/inputs.p1: ../inputs.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/inputs.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/inputs.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/inputs.p1 ../inputs.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/inputs.d ${OBJECTDIR}/_ext/1472/inputs.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/inputs.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1: ../event_consumerDualActionQueue.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1 ../event_consumerDualActionQueue.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.d ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/universalEvents.p1: ../universalEvents.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/universalEvents.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/universalEvents.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/universalEvents.p1 ../universalEvents.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/universalEvents.d ${OBJECTDIR}/_ext/1472/universalEvents.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/universalEvents.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/universalNv.p1: ../universalNv.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/universalNv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/universalNv.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/universalNv.p1 ../universalNv.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/universalNv.d ${OBJECTDIR}/_ext/1472/universalNv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/universalNv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/outputs.p1: ../outputs.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/outputs.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/outputs.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/outputs.p1 ../outputs.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/outputs.d ${OBJECTDIR}/_ext/1472/outputs.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/outputs.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/servo.p1: ../servo.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/servo.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/servo.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/servo.p1 ../servo.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/servo.d ${OBJECTDIR}/_ext/1472/servo.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/servo.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/bounce.p1: ../bounce.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/bounce.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/bounce.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/bounce.p1 ../bounce.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/bounce.d ${OBJECTDIR}/_ext/1472/bounce.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/bounce.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/analogue.p1: ../analogue.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/analogue.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/analogue.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/analogue.p1 ../analogue.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/analogue.d ${OBJECTDIR}/_ext/1472/analogue.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/analogue.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/boot.p1: ../../VLCBlib_PIC/boot.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/boot.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/boot.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/boot.p1 ../../VLCBlib_PIC/boot.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/boot.d ${OBJECTDIR}/_ext/1954642981/boot.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/boot.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/event_coe.p1: ../../VLCBlib_PIC/event_coe.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_coe.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_coe.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/event_coe.p1 ../../VLCBlib_PIC/event_coe.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/event_coe.d ${OBJECTDIR}/_ext/1954642981/event_coe.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/event_coe.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/mns.p1: ../../VLCBlib_PIC/mns.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/mns.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/mns.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/mns.p1 ../../VLCBlib_PIC/mns.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/mns.d ${OBJECTDIR}/_ext/1954642981/mns.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/mns.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/nv.p1: ../../VLCBlib_PIC/nv.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/nv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/nv.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/nv.p1 ../../VLCBlib_PIC/nv.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/nv.d ${OBJECTDIR}/_ext/1954642981/nv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/nv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/statusLeds2.p1: ../../VLCBlib_PIC/statusLeds2.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1 ../../VLCBlib_PIC/statusLeds2.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/statusLeds2.d ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/ticktime.p1: ../../VLCBlib_PIC/ticktime.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/ticktime.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/ticktime.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/ticktime.p1 ../../VLCBlib_PIC/ticktime.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/ticktime.d ${OBJECTDIR}/_ext/1954642981/ticktime.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/ticktime.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/timedResponse.p1: ../../VLCBlib_PIC/timedResponse.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/timedResponse.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/timedResponse.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/timedResponse.p1 ../../VLCBlib_PIC/timedResponse.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/timedResponse.d ${OBJECTDIR}/_ext/1954642981/timedResponse.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/timedResponse.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/vlcb.p1: ../../VLCBlib_PIC/vlcb.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/vlcb.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/vlcb.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/vlcb.p1 ../../VLCBlib_PIC/vlcb.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/vlcb.d ${OBJECTDIR}/_ext/1954642981/vlcb.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/vlcb.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/messageQueue.p1: ../../VLCBlib_PIC/messageQueue.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/messageQueue.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/messageQueue.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/messageQueue.p1 ../../VLCBlib_PIC/messageQueue.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/messageQueue.d ${OBJECTDIR}/_ext/1954642981/messageQueue.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/messageQueue.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/nvm.p1: ../../VLCBlib_PIC/nvm.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/nvm.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/nvm.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/nvm.p1 ../../VLCBlib_PIC/nvm.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/nvm.d ${OBJECTDIR}/_ext/1954642981/nvm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/nvm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/can18_ecan.p1: ../../VLCBlib_PIC/can18_ecan.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1 ../../VLCBlib_PIC/can18_ecan.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/can18_ecan.d ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/event_teach_large.p1: ../../VLCBlib_PIC/event_teach_large.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1 ../../VLCBlib_PIC/event_teach_large.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/event_teach_large.d ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1: ../../VLCBlib_PIC/event_producer_happening.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1 ../../VLCBlib_PIC/event_producer_happening.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/event_producer_happening.d ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1472/main.p1: ../main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/main.p1 ../main.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/main.d ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/digitalOut.p1: ../digitalOut.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/digitalOut.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/digitalOut.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/digitalOut.p1 ../digitalOut.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/digitalOut.d ${OBJECTDIR}/_ext/1472/digitalOut.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/digitalOut.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/inputs.p1: ../inputs.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/inputs.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/inputs.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/inputs.p1 ../inputs.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/inputs.d ${OBJECTDIR}/_ext/1472/inputs.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/inputs.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1: ../event_consumerDualActionQueue.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1 ../event_consumerDualActionQueue.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.d ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/event_consumerDualActionQueue.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/universalEvents.p1: ../universalEvents.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/universalEvents.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/universalEvents.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/universalEvents.p1 ../universalEvents.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/universalEvents.d ${OBJECTDIR}/_ext/1472/universalEvents.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/universalEvents.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/universalNv.p1: ../universalNv.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/universalNv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/universalNv.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/universalNv.p1 ../universalNv.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/universalNv.d ${OBJECTDIR}/_ext/1472/universalNv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/universalNv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/outputs.p1: ../outputs.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/outputs.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/outputs.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/outputs.p1 ../outputs.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/outputs.d ${OBJECTDIR}/_ext/1472/outputs.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/outputs.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/servo.p1: ../servo.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/servo.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/servo.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/servo.p1 ../servo.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/servo.d ${OBJECTDIR}/_ext/1472/servo.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/servo.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/bounce.p1: ../bounce.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/bounce.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/bounce.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/bounce.p1 ../bounce.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/bounce.d ${OBJECTDIR}/_ext/1472/bounce.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/bounce.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/analogue.p1: ../analogue.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/analogue.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/analogue.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1472/analogue.p1 ../analogue.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/analogue.d ${OBJECTDIR}/_ext/1472/analogue.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/analogue.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/boot.p1: ../../VLCBlib_PIC/boot.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/boot.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/boot.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/boot.p1 ../../VLCBlib_PIC/boot.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/boot.d ${OBJECTDIR}/_ext/1954642981/boot.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/boot.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/event_coe.p1: ../../VLCBlib_PIC/event_coe.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_coe.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_coe.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/event_coe.p1 ../../VLCBlib_PIC/event_coe.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/event_coe.d ${OBJECTDIR}/_ext/1954642981/event_coe.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/event_coe.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/mns.p1: ../../VLCBlib_PIC/mns.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/mns.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/mns.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/mns.p1 ../../VLCBlib_PIC/mns.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/mns.d ${OBJECTDIR}/_ext/1954642981/mns.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/mns.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/nv.p1: ../../VLCBlib_PIC/nv.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/nv.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/nv.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/nv.p1 ../../VLCBlib_PIC/nv.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/nv.d ${OBJECTDIR}/_ext/1954642981/nv.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/nv.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/statusLeds2.p1: ../../VLCBlib_PIC/statusLeds2.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1 ../../VLCBlib_PIC/statusLeds2.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/statusLeds2.d ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/statusLeds2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/ticktime.p1: ../../VLCBlib_PIC/ticktime.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/ticktime.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/ticktime.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/ticktime.p1 ../../VLCBlib_PIC/ticktime.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/ticktime.d ${OBJECTDIR}/_ext/1954642981/ticktime.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/ticktime.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/timedResponse.p1: ../../VLCBlib_PIC/timedResponse.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/timedResponse.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/timedResponse.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/timedResponse.p1 ../../VLCBlib_PIC/timedResponse.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/timedResponse.d ${OBJECTDIR}/_ext/1954642981/timedResponse.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/timedResponse.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/vlcb.p1: ../../VLCBlib_PIC/vlcb.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/vlcb.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/vlcb.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/vlcb.p1 ../../VLCBlib_PIC/vlcb.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/vlcb.d ${OBJECTDIR}/_ext/1954642981/vlcb.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/vlcb.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/messageQueue.p1: ../../VLCBlib_PIC/messageQueue.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/messageQueue.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/messageQueue.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/messageQueue.p1 ../../VLCBlib_PIC/messageQueue.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/messageQueue.d ${OBJECTDIR}/_ext/1954642981/messageQueue.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/messageQueue.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/nvm.p1: ../../VLCBlib_PIC/nvm.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/nvm.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/nvm.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/nvm.p1 ../../VLCBlib_PIC/nvm.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/nvm.d ${OBJECTDIR}/_ext/1954642981/nvm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/nvm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/can18_ecan.p1: ../../VLCBlib_PIC/can18_ecan.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1 ../../VLCBlib_PIC/can18_ecan.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/can18_ecan.d ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/can18_ecan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/event_teach_large.p1: ../../VLCBlib_PIC/event_teach_large.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1 ../../VLCBlib_PIC/event_teach_large.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/event_teach_large.d ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/event_teach_large.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1: ../../VLCBlib_PIC/event_producer_happening.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1954642981" 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26K80=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1 ../../VLCBlib_PIC/event_producer_happening.c 
	@-${MV} ${OBJECTDIR}/_ext/1954642981/event_producer_happening.d ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1954642981/event_producer_happening.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Universal-VLCB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Universal-VLCB.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=pickit5  -DXPRJ_PIC18F26K80=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto --CODEOFFSET=0x800        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Universal-VLCB.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/Universal-VLCB.X.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/Universal-VLCB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../../../MPLABworkspace/CBUS_PIC_Bootloader.X/dist/PIC18F26K80_canmio/production/CBUS_PIC_Bootloader.X.production.hex
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Universal-VLCB.X.${IMAGE_TYPE}.map  -DXPRJ_PIC18F26K80=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=800-EF80 -O1 -fasmfile -maddrqual=ignore -DHARDWARE=HW_CANMIO -xassembler-with-cpp -I"../" -I"../../VLCB-defs" -I"../../VLCBlib_PIC" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto --CODEOFFSET=0x800     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Universal-VLCB.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
	
	@echo "Creating unified hex file"
	@"C:/Program Files/Microchip/MPLABX/v6.20/mplab_platform/platform/../mplab_ide/modules/../../bin/hexmate" --edf="C:/Program Files/Microchip/MPLABX/v6.20/mplab_platform/platform/../mplab_ide/modules/../../dat/en_msgs.txt" ${DISTDIR}/Universal-VLCB.X.${IMAGE_TYPE}.hex ../../../MPLABworkspace/CBUS_PIC_Bootloader.X/dist/PIC18F26K80_canmio/production/CBUS_PIC_Bootloader.X.production.hex -odist/${CND_CONF}/production/Universal-VLCB.X.production.unified.hex

endif


# Subprojects
.build-subprojects:
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
	cd ../../../MPLABworkspace/CBUS_PIC_Bootloader.X && ${MAKE}  -f Makefile CONF=PIC18F26K80_canmio TYPE_IMAGE=DEBUG_RUN
else
	cd ../../../MPLABworkspace/CBUS_PIC_Bootloader.X && ${MAKE}  -f Makefile CONF=PIC18F26K80_canmio
endif


# Subprojects
.clean-subprojects:
	cd ../../../MPLABworkspace/CBUS_PIC_Bootloader.X && rm -rf "build/PIC18F26K80_canmio" "dist/PIC18F26K80_canmio"

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
