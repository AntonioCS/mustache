/* 
 * File:   minunit.h
 * http://www.jera.com/techinfo/jtns/jtn002.html
 *
 */

#ifndef MINUNIT_H
#define	MINUNIT_H

#ifdef	__cplusplus
extern "C" {
#endif

#define mu_assert(message, test) do { \
                                    asserts_run++; \
                                    if (!(test)) {\
                                        return message; \
                                    } \
                                 } while (0)
#define mu_run_test(test) do { \
                            char *message = test(); \
                            tests_run++; \
                            if (message) { \
                                return message; \
                            } \
                          } while (0)
    extern int tests_run;
    extern int asserts_run;

#define mu_main int main(int argc, char** argv) { \
    char *result = all_tests(); \
    if (result != 0) { \
        printf("%s\n", result); \
    } else { \
        printf("ALL TESTS PASSED\n"); \
    } \
    printf("Tests run: %d\n", tests_run); \
 \
    return result != 0; \
} \



#ifdef	__cplusplus
}
#endif

#endif	/* MINUNIT_H */

