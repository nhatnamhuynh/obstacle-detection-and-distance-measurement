#include "test_runner.h"
#include "main.h"

#if ENABLE_HARDWARE_TESTBENCH

extern void Input_RunHardwareTestbench(void);
extern void Display_RunHardwareTestbench(void);
extern void Actuators_RunHardwareTestbench(void);

void TestRunner_RunAll(void) {
    printf("\r\n==============================================\r\n");
    printf("     SYSTEM HARDWARE TESTBENCH STARTED        \r\n");
    printf("==============================================\r\n");

    Input_RunHardwareTestbench();
    Display_RunHardwareTestbench();
    Actuators_RunHardwareTestbench();

    printf("\r\n==============================================\r\n");
    printf("     ALL TESTCASES PASSED! STARTING APP...    \r\n");
    printf("==============================================\r\n\n");
}

#endif