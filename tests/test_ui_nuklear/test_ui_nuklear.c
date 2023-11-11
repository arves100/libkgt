/**
\file test_ui_nuklear.c
\brief Test nulkear/gpu ui
\author Arves100
\date 08/08/2022
*/

int main()
{
    KUI ui;
    KUICREATEPARAM param;
    struct nk_colorf bg;

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    param.width = 500;
    param.height = 500;
    param.inst = u_getinstance();
    param.style = 0;
#if _MSC_VER
    strncpy_s(param.title, _countof(param.title), "Nuklear keygen", KUI_MAX_BUFFER);
#else
    strncpy(param.title, "Nuklear keygen", KUI_MAX_BUFFER);
#endif

    assert(ui_create(&ui, &param));
    ui_instant_show(&ui);
    while (ui_loop(&ui))
    {

        /* GUI */
        if (nk_begin(ui.ctx, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
        {
            enum { EASY, HARD };
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(ui.ctx, 30, 80, 1);
            if (nk_button_label(ui.ctx, "button"))
            {
            }

            nk_layout_row_dynamic(ui.ctx, 30, 2);
            if (nk_option_label(ui.ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ui.ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(ui.ctx, 22, 1);
            nk_property_int(ui.ctx, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(ui.ctx, 20, 1);
            nk_label(ui.ctx, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ui.ctx, 25, 1);
            if (nk_combo_begin_color(ui.ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ui.ctx), 400))) {
                nk_layout_row_dynamic(ui.ctx, 120, 1);
                bg = nk_color_picker(ui.ctx, bg, NK_RGBA);
                nk_layout_row_dynamic(ui.ctx, 25, 1);
                bg.r = nk_propertyf(ui.ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
                bg.g = nk_propertyf(ui.ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
                bg.b = nk_propertyf(ui.ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
                bg.a = nk_propertyf(ui.ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
                nk_combo_end(ui.ctx);
            }
        }
        nk_end(ui.ctx);

        gpu_begin(&ui.gpu, bg);
        ui_render(&ui);
        gpu_present(&ui.gpu);
    }
    ui_free(&ui);

    return 0;
}

