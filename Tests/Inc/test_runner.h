#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#define ENABLE_HARDWARE_TESTBENCH 1

#if ENABLE_HARDWARE_TESTBENCH
    void TestRunner_RunAll(void);
#endif

#endif