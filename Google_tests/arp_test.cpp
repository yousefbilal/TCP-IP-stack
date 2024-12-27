#include <gtest/gtest.h>
#include "../include/arp.h"

TEST(ArpTest, TranslationTable) {
    translation_table* table = (translation_table*)malloc(sizeof(translation_table));
    init_translation_table(table);
    unsigned char data[6] = {5,5,5,5,5,5};
    insert(table, 19, data);
    table_entry* val = search(table, 19);
    EXPECT_EQ(0, memcmp(val->value, data, 6));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}