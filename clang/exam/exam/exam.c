/* exam.c
 * 入口主程序
 */

#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "problem.h"
#include "ui.h"

int main(int argc, char *argv[])
{
  if (argc > 1) {
    // process_args
  }
  ui_login();
  return 0;
}
