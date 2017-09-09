#include "khash.h"
#include <stdio.h>
KHASH_MAP_INIT_INT(32, int)

int main(int argc, char *argv[]) {

   if (argc == 2) {
      int ret, is_missing;
      khiter_t k;
      khash_t(32) *h = kh_init(32);

      for (int i = 0; i < atoi(argv[1]); i++) {
         k = kh_put(32, h, i, &ret);
         kh_value(h, k) = i;
      }
      /*
      for (int i = 0; i < 10; i++) {
         k = kh_get(32, h, i);
         if (k == kh_end(h)) {
            printf("not found\n");
         } else {
            printf("key = %d\n", kh_val(h, k));
         }
      }
      */
      kh_destroy(32, h);
   } else {
      fprintf(stderr, "Usage ./time_test_khash <size>\n");
   }
   return 0;
}