/**
   test_grid_exec.c

   invoke src/run_cvmsi_txt
**/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <getopt.h>
#include "unittest_defs.h"
#include "test_helper.h"
#include "test_grid_exec.h"

int CVMSI_GRID_TESTS=1;

int test_cvmsi_grid_depth()
{
  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  printf("Test: model with grid in elevation mode\n");

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test_latlons_gd.txt");
  sprintf(outfile, "%s/%s", currentdir, "test-cvmsi-grid-extract-depth.out");
  sprintf(reffile, "%s/%s", currentdir, "./ref/test-cvmsi-grid-extract-depth.ref");

  if (test_assert_int(runCVMSI(BIN_DIR, MODEL_DIR,infile, outfile,
                                        UCVM_COORD_GEO_DEPTH ), 0) != 0) {
    printf("cvmsi failure\n");
    return(1);
  }

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    printf("unmatched result\n");
    printf("%s\n",outfile);
    printf("%s\n",reffile);
    return(1);
  }

  unlink(outfile);
  printf("PASS\n");
  return(0);
}

int suite_grid_exec(const char *xmldir)
{
  suite_t suite;
  char logfile[256];
  FILE *lf = NULL;

  /* Setup test suite */
  strcpy(suite.suite_name, "suite_grid_exec");
  suite.num_tests = CVMSI_GRID_TESTS;
  suite.tests = malloc(suite.num_tests * sizeof(test_t));
  if (suite.tests == NULL) {
    fprintf(stderr, "Failed to alloc test structure\n");
    return(1);
  }
  test_get_time(&suite.exec_time);

  /* Setup test cases */

  strcpy(suite.tests[0].test_name, "test_cvmsi_grid_depth");
  suite.tests[0].test_func = &test_cvmsi_grid_depth;
  suite.tests[0].elapsed_time = 0.0;

  if (test_run_suite(&suite) != 0) {
    fprintf(stderr, "Failed to execute tests\n");
    return(1);
  }

  if (xmldir != NULL) {
    sprintf(logfile, "%s/%s.xml", xmldir, suite.suite_name);
    lf = init_log(logfile);
    if (lf == NULL) {
      fprintf(stderr, "Failed to initialize logfile\n");
      return(1);
    }
    
    if (write_log(lf, &suite) != 0) {
      fprintf(stderr, "Failed to write test log\n");
      return(1);
    }
    
    close_log(lf);
  }

  free(suite.tests);

  return 0;
}