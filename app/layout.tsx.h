/** This file is generated from layout.tsx */
#include <ui.h>
#include "../.lcui/build/[modules]/@lcui/react-icons/dist/style.css.h"
#include "global.css.h"

// UTF-8 encoded string from: CE
static const unsigned char widget_text_0[] = {0x43, 0x45, 0};
// UTF-8 encoded string from: CEP%20Management
static const unsigned char widget_text_1[] = {0x43, 0x45, 0x50, 0x20, 0x4d, 0x61, 0x6e, 0x61, 0x67, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0};
// UTF-8 encoded string from: Autorit%C3%A9%20%C3%89lectorale
static const unsigned char widget_text_2[] = {0x41, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x74, 0xc3, 0xa9, 0x20, 0xc3, 0x89, 0x6c, 0x65, 0x63, 0x74, 0x6f, 0x72, 0x61, 0x6c, 0x65, 0};
// UTF-8 encoded string from: %EF%91%A2
static const unsigned char widget_text_3[] = {0xef, 0x91, 0xa2, 0};
// UTF-8 encoded string from: Dashboard
static const unsigned char widget_text_4[] = {0x44, 0x61, 0x73, 0x68, 0x62, 0x6f, 0x61, 0x72, 0x64, 0};
// UTF-8 encoded string from: %EF%90%8B
static const unsigned char widget_text_5[] = {0xef, 0x90, 0x8b, 0};
// UTF-8 encoded string from: Partis%20Politiques
static const unsigned char widget_text_6[] = {0x50, 0x61, 0x72, 0x74, 0x69, 0x73, 0x20, 0x50, 0x6f, 0x6c, 0x69, 0x74, 0x69, 0x71, 0x75, 0x65, 0x73, 0};
// UTF-8 encoded string from: %EF%97%8D
static const unsigned char widget_text_7[] = {0xef, 0x97, 0x8d, 0};
// UTF-8 encoded string from: Candidats
static const unsigned char widget_text_8[] = {0x43, 0x61, 0x6e, 0x64, 0x69, 0x64, 0x61, 0x74, 0x73, 0};
// UTF-8 encoded string from: %EF%96%BD
static const unsigned char widget_text_9[] = {0xef, 0x96, 0xbd, 0};
// UTF-8 encoded string from: %C3%89lecteurs
static const unsigned char widget_text_10[] = {0xc3, 0x89, 0x6c, 0x65, 0x63, 0x74, 0x65, 0x75, 0x72, 0x73, 0};
// UTF-8 encoded string from: %EE%88%9C
static const unsigned char widget_text_11[] = {0xee, 0x88, 0x9c, 0};
// UTF-8 encoded string from: Bureaux%20de%20Vote
static const unsigned char widget_text_12[] = {0x42, 0x75, 0x72, 0x65, 0x61, 0x75, 0x78, 0x20, 0x64, 0x65, 0x20, 0x56, 0x6f, 0x74, 0x65, 0};
// UTF-8 encoded string from: %EE%B8%A8
static const unsigned char widget_text_13[] = {0xee, 0xb8, 0xa8, 0};
// UTF-8 encoded string from: Enregistrer%20un%20Vote
static const unsigned char widget_text_14[] = {0x45, 0x6e, 0x72, 0x65, 0x67, 0x69, 0x73, 0x74, 0x72, 0x65, 0x72, 0x20, 0x75, 0x6e, 0x20, 0x56, 0x6f, 0x74, 0x65, 0};
// UTF-8 encoded string from: %EF%8C%BF
static const unsigned char widget_text_15[] = {0xef, 0x8c, 0xbf, 0};
// UTF-8 encoded string from: R%C3%A9sultats
static const unsigned char widget_text_16[] = {0x52, 0xc3, 0xa9, 0x73, 0x75, 0x6c, 0x74, 0x61, 0x74, 0x73, 0};
// UTF-8 encoded string from: %C3%89lections%202026%20%E2%80%94%20Ha%C3%AFti
static const unsigned char widget_text_17[] = {0xc3, 0x89, 0x6c, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x32, 0x30, 0x32, 0x36, 0x20, 0xe2, 0x80, 0x94, 0x20, 0x48, 0x61, 0xc3, 0xaf, 0x74, 0x69, 0};
// UTF-8 encoded string from: Electoral%20Management%20System
static const unsigned char widget_text_18[] = {0x45, 0x6c, 0x65, 0x63, 0x74, 0x6f, 0x72, 0x61, 0x6c, 0x20, 0x4d, 0x61, 0x6e, 0x61, 0x67, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x20, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0};
// UTF-8 encoded string from: Conseil%20%C3%89lectoral%20Provisoire
static const unsigned char widget_text_19[] = {0x43, 0x6f, 0x6e, 0x73, 0x65, 0x69, 0x6c, 0x20, 0xc3, 0x89, 0x6c, 0x65, 0x63, 0x74, 0x6f, 0x72, 0x61, 0x6c, 0x20, 0x50, 0x72, 0x6f, 0x76, 0x69, 0x73, 0x6f, 0x69, 0x72, 0x65, 0};

typedef struct root_layout_react_state {
        char empty;
} root_layout_react_state_t;

typedef struct root_layout_react {
        root_layout_react_state_t state;
} root_layout_react_t;

static ui_widget_prototype_t *root_layout_proto;

static void root_layout_init_prototype(void)
{
        root_layout_proto = ui_create_widget_prototype("root_layout", NULL);
}

static void root_layout_load_template(ui_widget_t *parent)
{
        ui_widget_t *w[42];

        ui_widget_add_class(parent, "w-full h-full flex flex-row bg-background");
        w[0] = ui_create_widget(NULL);
        ui_widget_add_class(w[0], "h-full bg-surface-container border-r border-outline-variant flex flex-col py-6");
        ui_widget_set_style_string(w[0], "width", "24%");
        ui_widget_set_style_string(w[0], "min-width", "180px");
        ui_widget_set_style_string(w[0], "max-width", "270px");
        w[1] = ui_create_widget(NULL);
        ui_widget_add_class(w[1], "px-4 mb-6 flex flex-row items-center");
        w[2] = ui_create_widget(NULL);
        ui_widget_add_class(w[2], "rounded-full bg-primary flex items-center justify-center");
        ui_widget_set_style_string(w[2], "width", "40px");
        ui_widget_set_style_string(w[2], "height", "40px");
        w[3] = ui_create_widget("text");
        ui_widget_add_class(w[3], "text-on-primary font-bold");
        ui_widget_set_text(w[3], (const char*)widget_text_0);
        ui_widget_append(w[2], w[3]);
        w[4] = ui_create_widget(NULL);
        ui_widget_add_class(w[4], "ml-2 flex flex-col");
        w[5] = ui_create_widget("text");
        ui_widget_add_class(w[5], "text-headline-sm font-bold text-on-surface");
        ui_widget_set_text(w[5], (const char*)widget_text_1);
        w[6] = ui_create_widget("text");
        ui_widget_add_class(w[6], "text-label-md text-secondary");
        ui_widget_set_text(w[6], (const char*)widget_text_2);
        ui_widget_append(w[4], w[5]);
        ui_widget_append(w[4], w[6]);
        ui_widget_append(w[1], w[2]);
        ui_widget_append(w[1], w[4]);
        w[7] = ui_create_widget("scrollarea");
        ui_widget_add_class(w[7], "flex-1 w-full");
        ui_widget_set_style_string(w[7], "min-height", "0px");
        w[8] = ui_create_widget("scrollarea-content");
        ui_widget_add_class(w[8], "w-full");
        w[9] = ui_create_widget(NULL);
        ui_widget_add_class(w[9], "flex flex-col px-2");
        w[10] = ui_create_widget("router-link");
        ui_widget_set_attr(w[10], "to", "/");
        ui_widget_set_attr(w[10], "active-class", "bg-surface-container-high text-primary font-bold");
        ui_widget_set_attr(w[10], "exact", "exact");
        ui_widget_set_attr(w[10], "exact-active-class", "");
        ui_widget_add_class(w[10], "flex flex-row items-center px-4 py-2 mb-1 rounded-lg text-secondary");
        w[11] = ui_create_widget("text");
        ui_widget_add_class(w[11], "fui-icon-regular");
        ui_widget_set_style_string(w[11], "font-size", "18px");
        ui_widget_set_text(w[11], (const char*)widget_text_3);
        w[12] = ui_create_widget("text");
        ui_widget_add_class(w[12], "text-label-md ml-2");
        ui_widget_set_text(w[12], (const char*)widget_text_4);
        ui_widget_append(w[10], w[11]);
        ui_widget_append(w[10], w[12]);
        w[13] = ui_create_widget("router-link");
        ui_widget_set_attr(w[13], "to", "/partis");
        ui_widget_set_attr(w[13], "active-class", "bg-surface-container-high text-primary font-bold");
        ui_widget_set_attr(w[13], "exact", "");
        ui_widget_set_attr(w[13], "exact-active-class", "");
        ui_widget_add_class(w[13], "flex flex-row items-center px-4 py-2 mb-1 rounded-lg text-secondary");
        w[14] = ui_create_widget("text");
        ui_widget_add_class(w[14], "fui-icon-regular");
        ui_widget_set_style_string(w[14], "font-size", "18px");
        ui_widget_set_text(w[14], (const char*)widget_text_5);
        w[15] = ui_create_widget("text");
        ui_widget_add_class(w[15], "text-label-md ml-2");
        ui_widget_set_text(w[15], (const char*)widget_text_6);
        ui_widget_append(w[13], w[14]);
        ui_widget_append(w[13], w[15]);
        w[16] = ui_create_widget("router-link");
        ui_widget_set_attr(w[16], "to", "/candidats");
        ui_widget_set_attr(w[16], "active-class", "bg-surface-container-high text-primary font-bold");
        ui_widget_set_attr(w[16], "exact", "");
        ui_widget_set_attr(w[16], "exact-active-class", "");
        ui_widget_add_class(w[16], "flex flex-row items-center px-4 py-2 mb-1 rounded-lg text-secondary");
        w[17] = ui_create_widget("text");
        ui_widget_add_class(w[17], "fui-icon-regular");
        ui_widget_set_style_string(w[17], "font-size", "18px");
        ui_widget_set_text(w[17], (const char*)widget_text_7);
        w[18] = ui_create_widget("text");
        ui_widget_add_class(w[18], "text-label-md ml-2");
        ui_widget_set_text(w[18], (const char*)widget_text_8);
        ui_widget_append(w[16], w[17]);
        ui_widget_append(w[16], w[18]);
        w[19] = ui_create_widget("router-link");
        ui_widget_set_attr(w[19], "to", "/electeurs");
        ui_widget_set_attr(w[19], "active-class", "bg-surface-container-high text-primary font-bold");
        ui_widget_set_attr(w[19], "exact", "");
        ui_widget_set_attr(w[19], "exact-active-class", "");
        ui_widget_add_class(w[19], "flex flex-row items-center px-4 py-2 mb-1 rounded-lg text-secondary");
        w[20] = ui_create_widget("text");
        ui_widget_add_class(w[20], "fui-icon-regular");
        ui_widget_set_style_string(w[20], "font-size", "18px");
        ui_widget_set_text(w[20], (const char*)widget_text_9);
        w[21] = ui_create_widget("text");
        ui_widget_add_class(w[21], "text-label-md ml-2");
        ui_widget_set_text(w[21], (const char*)widget_text_10);
        ui_widget_append(w[19], w[20]);
        ui_widget_append(w[19], w[21]);
        w[22] = ui_create_widget("router-link");
        ui_widget_set_attr(w[22], "to", "/bureaux");
        ui_widget_set_attr(w[22], "active-class", "bg-surface-container-high text-primary font-bold");
        ui_widget_set_attr(w[22], "exact", "");
        ui_widget_set_attr(w[22], "exact-active-class", "");
        ui_widget_add_class(w[22], "flex flex-row items-center px-4 py-2 mb-1 rounded-lg text-secondary");
        w[23] = ui_create_widget("text");
        ui_widget_add_class(w[23], "fui-icon-regular");
        ui_widget_set_style_string(w[23], "font-size", "18px");
        ui_widget_set_text(w[23], (const char*)widget_text_11);
        w[24] = ui_create_widget("text");
        ui_widget_add_class(w[24], "text-label-md ml-2");
        ui_widget_set_text(w[24], (const char*)widget_text_12);
        ui_widget_append(w[22], w[23]);
        ui_widget_append(w[22], w[24]);
        w[25] = ui_create_widget("router-link");
        ui_widget_set_attr(w[25], "to", "/vote");
        ui_widget_set_attr(w[25], "active-class", "bg-surface-container-high text-primary font-bold");
        ui_widget_set_attr(w[25], "exact", "");
        ui_widget_set_attr(w[25], "exact-active-class", "");
        ui_widget_add_class(w[25], "flex flex-row items-center px-4 py-2 mb-1 rounded-lg text-secondary");
        w[26] = ui_create_widget("text");
        ui_widget_add_class(w[26], "fui-icon-regular");
        ui_widget_set_style_string(w[26], "font-size", "18px");
        ui_widget_set_text(w[26], (const char*)widget_text_13);
        w[27] = ui_create_widget("text");
        ui_widget_add_class(w[27], "text-label-md ml-2");
        ui_widget_set_text(w[27], (const char*)widget_text_14);
        ui_widget_append(w[25], w[26]);
        ui_widget_append(w[25], w[27]);
        w[28] = ui_create_widget("router-link");
        ui_widget_set_attr(w[28], "to", "/resultats");
        ui_widget_set_attr(w[28], "active-class", "bg-surface-container-high text-primary font-bold");
        ui_widget_set_attr(w[28], "exact", "");
        ui_widget_set_attr(w[28], "exact-active-class", "");
        ui_widget_add_class(w[28], "flex flex-row items-center px-4 py-2 mb-1 rounded-lg text-secondary");
        w[29] = ui_create_widget("text");
        ui_widget_add_class(w[29], "fui-icon-regular");
        ui_widget_set_style_string(w[29], "font-size", "18px");
        ui_widget_set_text(w[29], (const char*)widget_text_15);
        w[30] = ui_create_widget("text");
        ui_widget_add_class(w[30], "text-label-md ml-2");
        ui_widget_set_text(w[30], (const char*)widget_text_16);
        ui_widget_append(w[28], w[29]);
        ui_widget_append(w[28], w[30]);
        ui_widget_append(w[9], w[10]);
        ui_widget_append(w[9], w[13]);
        ui_widget_append(w[9], w[16]);
        ui_widget_append(w[9], w[19]);
        ui_widget_append(w[9], w[22]);
        ui_widget_append(w[9], w[25]);
        ui_widget_append(w[9], w[28]);
        ui_widget_append(w[8], w[9]);
        w[31] = ui_create_widget("scrollbar");
        ui_widget_append(w[7], w[8]);
        ui_widget_append(w[7], w[31]);
        w[32] = ui_create_widget(NULL);
        ui_widget_add_class(w[32], "px-4 mt-4 pt-4 border-t border-outline-variant");
        w[33] = ui_create_widget("text");
        ui_widget_add_class(w[33], "text-label-sm text-secondary");
        ui_widget_set_text(w[33], (const char*)widget_text_17);
        ui_widget_append(w[32], w[33]);
        ui_widget_append(w[0], w[1]);
        ui_widget_append(w[0], w[7]);
        ui_widget_append(w[0], w[32]);
        w[34] = ui_create_widget(NULL);
        ui_widget_add_class(w[34], "flex-1 flex flex-col h-full");
        ui_widget_set_style_string(w[34], "min-width", "0px");
        w[35] = ui_create_widget(NULL);
        ui_widget_add_class(w[35], "flex flex-row flex-wrap justify-between items-center px-6 bg-surface border-b border-outline-variant");
        ui_widget_set_style_string(w[35], "min-height", "64px");
        w[36] = ui_create_widget("text");
        ui_widget_add_class(w[36], "text-headline-md text-primary font-semibold");
        ui_widget_set_text(w[36], (const char*)widget_text_18);
        w[37] = ui_create_widget("text");
        ui_widget_add_class(w[37], "text-label-md text-secondary");
        ui_widget_set_text(w[37], (const char*)widget_text_19);
        ui_widget_append(w[35], w[36]);
        ui_widget_append(w[35], w[37]);
        w[38] = ui_create_widget("scrollarea");
        ui_widget_add_class(w[38], "flex-1 w-full");
        ui_widget_set_style_string(w[38], "min-height", "0px");
        w[39] = ui_create_widget("scrollarea-content");
        ui_widget_add_class(w[39], "w-full");
        w[40] = ui_create_widget("router-view");
        ui_widget_append(w[39], w[40]);
        w[41] = ui_create_widget("scrollbar");
        ui_widget_append(w[38], w[39]);
        ui_widget_append(w[38], w[41]);
        ui_widget_append(w[34], w[35]);
        ui_widget_append(w[34], w[38]);
        ui_widget_append(parent, w[0]);
        ui_widget_append(parent, w[34]);
}

static void root_layout_react_update(ui_widget_t *w)
{
        root_layout_react_t *_that = ui_widget_get_data(w, root_layout_proto);
}

static void root_layout_react_init(ui_widget_t *w)
{
        root_layout_react_t *_that = ui_widget_get_data(w, root_layout_proto);
        root_layout_load_template(w);
}

static void root_layout_react_destroy(ui_widget_t *w)
{
}

void ui_load_root_layout_resources(void)
{
        ui_load_css_string(css_str_style, "style.css");
        ui_load_css_string(css_str_global, "global.css");
}
