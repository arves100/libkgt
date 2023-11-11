/**
\file test_ui_nativeskinned.c
\brief Test native ui with a skinned image
\author Arves100
\date 08/08/2022
*/

int main()
{
    KUI ui;
    KBITMAP bmp;
    KCREATEPARAM param;

    assert(rez_getbitmap(u_getinstance(), IDB_BACKGROUND, &bmp));

    ui.style = KUI_STYLE_IMAGE_BG | KUI_STYLE_NO_BORDER;
    ui.font_color = RGB(255, 255, 255);

    assert(ui_create(&ui, u_getinstance(), bmp.width, bmp.height, bmp.handle));
    assert(ui_add_label(&ui, 10, 0, "Test test"));
    ui_instant_show(&ui);
    while (ui_loop(&ui));
    ui_free(&ui);

    rez_freebitmap(&bmp);

    return 0;
}
