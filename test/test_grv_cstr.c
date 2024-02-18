#include "grv/grv_test.h"
#include "grv/grv_cstr.h"


GRV_TEST_BEGIN(grv_cstr_eq)
    GRV_TEST_TRUE(grv_cstr_eq("Hallo", "Hallo")); 
    GRV_TEST_FALSE(grv_cstr_eq("Hallo", "Hall"));
    GRV_TEST_FALSE(grv_cstr_eq("Hallo", "Halloooo"));
    GRV_TEST_FALSE(grv_cstr_eq("Hallo", ""));
    GRV_TEST_TRUE(grv_cstr_eq("", ""));
GRV_TEST_END()

int main(void) {
    GRV_TEST_INIT(grv_cstr);
    GRV_TEST_RUN(grv_cstr_eq);
    GRV_TEST_FINISH();
}
