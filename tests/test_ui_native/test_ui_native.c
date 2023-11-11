/**
\file test_ui_native.c
\brief Test native ui
\author Arves100
\date 08/08/2022
*/

int main()
{
    KUI ui;
    KCREATEPARAM param;

    assert(ui_create(&ui, u_getinstance(), 0, 0, NULL) == true);
    assert(ui_change_title(&ui, "TEST Keygen") == true);
    assert(ui_add_label(&ui, 100, 0, "Test test"));
    //assert(ui_add_edit(&ui, 100, 200, 40, NULL));
    ui_instant_show(&ui);
    while (ui_loop(&ui));
    ui_free(&ui);
    return 0;
}
