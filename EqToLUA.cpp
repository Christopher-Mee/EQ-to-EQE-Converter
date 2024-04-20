#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  bool isFirstLine = true;
  char tmp[128];
  char abrvFilterName[][3] = {"LS", "HS", "PK"};
  char fullFilterName[][20] = {"lowshelf", "highshelf", "eq"};

  while (NULL != fgets(tmp, sizeof(tmp), stdin)) {
    if (isFirstLine) {
      double preamp;
      int ret = sscanf(tmp, "Preamp: %lf dB\n", &preamp);
      if (1 == ret) {
        printf("return {\n");
        printf("  {\n");
        printf("    name = \"preamp\",\n");
        printf("    gain = %lf,\n", preamp);
        printf("  },\n");
      } else {
        break;
      }
      isFirstLine = false;
    } else {
      int filter_num;
      char filter_type[3] = "eq";
      double fc, gain, q;
      int ret = sscanf(tmp, "Filter %d: ON %s Fc %lf Hz Gain %lf dB Q %lf\n",
                       &filter_num, filter_type, &fc, &gain,
                       &q);  // ignore type '%*[A-Za-z0-9]'
      if (ret == 5) {
        // line was recognized

        // Need to read band type ex lowshelf or peak
        char
            abrToFull[sizeof(fullFilterName[0]) / sizeof(fullFilterName[0][0])];
        strcpy(abrToFull, "eq"); // set default value

        for (int i = 0; i < sizeof(abrvFilterName) / sizeof(abrvFilterName[0]);
             i++) {
          if (0 == strcmp(filter_type, abrvFilterName[i])) {
            strcpy(abrToFull, fullFilterName[i]);
            break;
          }
        }

        printf("  {\n");
        printf("    name = \"%s\",\n", abrToFull);
        printf("    frequency = %lf,\n", fc);
        printf("    gain = %lf,\n", gain);
        printf("    Q = %lf,\n", q);
        printf("  },\n");
      } else {
        // line was not recognized
        // printf ("ERROR: %d, %s\n", ret, tmp);
      }
    }
  }
  printf("}\n");
}
