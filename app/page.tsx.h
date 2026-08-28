/** This file is generated from page.tsx */
#include <ui.h>

// UTF-8 encoded string from: Tableau%20de%20bord
static const unsigned char widget_text_0[] = {0x54, 0x61, 0x62, 0x6c, 0x65, 0x61, 0x75, 0x20, 0x64, 0x65, 0x20, 0x62, 0x6f, 0x72, 0x64, 0};
// UTF-8 encoded string from: Aper%C3%A7u%20g%C3%A9n%C3%A9ral%20du%20syst%C3%A8me%20%C3%A9lectoral.
static const unsigned char widget_text_1[] = {0x41, 0x70, 0x65, 0x72, 0xc3, 0xa7, 0x75, 0x20, 0x67, 0xc3, 0xa9, 0x6e, 0xc3, 0xa9, 0x72, 0x61, 0x6c, 0x20, 0x64, 0x75, 0x20, 0x73, 0x79, 0x73, 0x74, 0xc3, 0xa8, 0x6d, 0x65, 0x20, 0xc3, 0xa9, 0x6c, 0x65, 0x63, 0x74, 0x6f, 0x72, 0x61, 0x6c, 0x2e, 0};
// UTF-8 encoded string from: %EE%84%90
static const unsigned char widget_text_2[] = {0xee, 0x84, 0x90, 0};
// UTF-8 encoded string from: Total%20%C3%89lecteurs
static const unsigned char widget_text_3[] = {0x54, 0x6f, 0x74, 0x61, 0x6c, 0x20, 0xc3, 0x89, 0x6c, 0x65, 0x63, 0x74, 0x65, 0x75, 0x72, 0x73, 0};
// UTF-8 encoded string from: %EF%96%A8
static const unsigned char widget_text_4[] = {0xef, 0x96, 0xa8, 0};
// UTF-8 encoded string from: 0
static const unsigned char widget_text_5[] = {0x30, 0};
// UTF-8 encoded string from: Partis%20Inscrits
static const unsigned char widget_text_6[] = {0x50, 0x61, 0x72, 0x74, 0x69, 0x73, 0x20, 0x49, 0x6e, 0x73, 0x63, 0x72, 0x69, 0x74, 0x73, 0};
// UTF-8 encoded string from: %EF%90%8B
static const unsigned char widget_text_7[] = {0xef, 0x90, 0x8b, 0};
// UTF-8 encoded string from: 0
static const unsigned char widget_text_8[] = {0x30, 0};
// UTF-8 encoded string from: Bureaux%20de%20Vote
static const unsigned char widget_text_9[] = {0x42, 0x75, 0x72, 0x65, 0x61, 0x75, 0x78, 0x20, 0x64, 0x65, 0x20, 0x56, 0x6f, 0x74, 0x65, 0};
// UTF-8 encoded string from: %EE%88%9C
static const unsigned char widget_text_10[] = {0xee, 0x88, 0x9c, 0};
// UTF-8 encoded string from: 0
static const unsigned char widget_text_11[] = {0x30, 0};
// UTF-8 encoded string from: Votes%20Enregistr%C3%A9s
static const unsigned char widget_text_12[] = {0x56, 0x6f, 0x74, 0x65, 0x73, 0x20, 0x45, 0x6e, 0x72, 0x65, 0x67, 0x69, 0x73, 0x74, 0x72, 0xc3, 0xa9, 0x73, 0};
// UTF-8 encoded string from: %EE%A6%9F
static const unsigned char widget_text_13[] = {0xee, 0xa6, 0x9f, 0};
// UTF-8 encoded string from: 0
static const unsigned char widget_text_14[] = {0x30, 0};
// UTF-8 encoded string from: R%C3%A9sultats%20nationaux%20(provisoires)
static const unsigned char widget_text_15[] = {0x52, 0xc3, 0xa9, 0x73, 0x75, 0x6c, 0x74, 0x61, 0x74, 0x73, 0x20, 0x6e, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x61, 0x75, 0x78, 0x20, 0x28, 0x70, 0x72, 0x6f, 0x76, 0x69, 0x73, 0x6f, 0x69, 0x72, 0x65, 0x73, 0x29, 0};
// UTF-8 encoded string from: Candidat
static const unsigned char widget_text_16[] = {0x43, 0x61, 0x6e, 0x64, 0x69, 0x64, 0x61, 0x74, 0};
// UTF-8 encoded string from: Parti%20%2F%20Type
static const unsigned char widget_text_17[] = {0x50, 0x61, 0x72, 0x74, 0x69, 0x20, 0x2f, 0x20, 0x54, 0x79, 0x70, 0x65, 0};
// UTF-8 encoded string from: Voix
static const unsigned char widget_text_18[] = {0x56, 0x6f, 0x69, 0x78, 0};
// UTF-8 encoded string from: %25
static const unsigned char widget_text_19[] = {0x25, 0};
// UTF-8 encoded string from: Activit%C3%A9%20R%C3%A9cente
static const unsigned char widget_text_20[] = {0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0xc3, 0xa9, 0x20, 0x52, 0xc3, 0xa9, 0x63, 0x65, 0x6e, 0x74, 0x65, 0};

typedef struct {
        ui_widget_t *ref_0;
        ui_widget_t *date_maj;
        ui_widget_t *nb_electeurs;
        ui_widget_t *nb_partis;
        ui_widget_t *nb_bv;
        ui_widget_t *nb_votes;
        ui_widget_t *participation;
        ui_widget_t *resultats;
        ui_widget_t *activite;
} root_page_refs_t;

typedef struct root_page_react_state {
        char empty;
} root_page_react_state_t;

typedef struct root_page_react {
        root_page_react_state_t state;
        root_page_refs_t refs;
} root_page_react_t;

static ui_widget_prototype_t *root_page_proto;

static void root_page_init_prototype(void)
{
        root_page_proto = ui_create_widget_prototype("root_page", NULL);
}

static void root_page_load_template(ui_widget_t *parent, root_page_refs_t *refs)
{
        ui_widget_t *w[33];

        ui_widget_add_class(parent, "w-full p-6 flex flex-col bg-background");
        w[0] = ui_create_widget(NULL);
        ui_widget_add_class(w[0], "flex flex-row justify-between items-center mb-6");
        w[1] = ui_create_widget(NULL);
        ui_widget_add_class(w[1], "flex flex-col");
        w[2] = ui_create_widget("text");
        ui_widget_add_class(w[2], "text-headline-lg text-on-surface");
        ui_widget_set_text(w[2], (const char*)widget_text_0);
        w[3] = ui_create_widget("text");
        ui_widget_add_class(w[3], "text-body-sm text-secondary mt-1");
        ui_widget_set_text(w[3], (const char*)widget_text_1);
        ui_widget_append(w[1], w[2]);
        ui_widget_append(w[1], w[3]);
        w[4] = ui_create_widget(NULL);
        ui_widget_add_class(w[4], "flex flex-row items-center");
        refs->ref_0 = ui_create_widget("button");
        ui_widget_add_class(refs->ref_0, "bg-surface border border-outline-variant rounded-lg px-3 py-1 mr-2");
        w[5] = ui_create_widget("text");
        ui_widget_add_class(w[5], "fui-icon-regular");
        ui_widget_set_style_string(w[5], "font-size", "14px");
        ui_widget_set_text(w[5], (const char*)widget_text_2);
        ui_widget_append(refs->ref_0, w[5]);
        refs->date_maj = ui_create_widget("text");
        ui_widget_add_class(refs->date_maj, "text-label-sm text-secondary");
        ui_widget_append(w[4], refs->ref_0);
        ui_widget_append(w[4], refs->date_maj);
        ui_widget_append(w[0], w[1]);
        ui_widget_append(w[0], w[4]);
        w[6] = ui_create_widget(NULL);
        ui_widget_add_class(w[6], "w-full flex flex-row flex-wrap mb-4");
        w[7] = ui_create_widget(NULL);
        ui_widget_add_class(w[7], "bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col justify-between");
        ui_widget_set_style_string(w[7], "width", "23%");
        ui_widget_set_style_string(w[7], "min-width", "170px");
        ui_widget_set_style_string(w[7], "flex-grow", "1");
        w[8] = ui_create_widget(NULL);
        ui_widget_add_class(w[8], "flex flex-row justify-between items-center mb-2");
        w[9] = ui_create_widget("text");
        ui_widget_add_class(w[9], "text-label-md text-secondary");
        ui_widget_set_text(w[9], (const char*)widget_text_3);
        w[10] = ui_create_widget("text");
        ui_widget_add_class(w[10], "fui-icon-regular text-primary");
        ui_widget_set_style_string(w[10], "font-size", "18px");
        ui_widget_set_text(w[10], (const char*)widget_text_4);
        ui_widget_append(w[8], w[9]);
        ui_widget_append(w[8], w[10]);
        refs->nb_electeurs = ui_create_widget("text");
        ui_widget_add_class(refs->nb_electeurs, "text-headline-lg text-on-surface");
        ui_widget_set_text(refs->nb_electeurs, (const char*)widget_text_5);
        ui_widget_append(w[7], w[8]);
        ui_widget_append(w[7], refs->nb_electeurs);
        w[11] = ui_create_widget(NULL);
        ui_widget_add_class(w[11], "bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col justify-between");
        ui_widget_set_style_string(w[11], "width", "23%");
        ui_widget_set_style_string(w[11], "min-width", "170px");
        ui_widget_set_style_string(w[11], "flex-grow", "1");
        w[12] = ui_create_widget(NULL);
        ui_widget_add_class(w[12], "flex flex-row justify-between items-center mb-2");
        w[13] = ui_create_widget("text");
        ui_widget_add_class(w[13], "text-label-md text-secondary");
        ui_widget_set_text(w[13], (const char*)widget_text_6);
        w[14] = ui_create_widget("text");
        ui_widget_add_class(w[14], "fui-icon-regular text-primary");
        ui_widget_set_style_string(w[14], "font-size", "18px");
        ui_widget_set_text(w[14], (const char*)widget_text_7);
        ui_widget_append(w[12], w[13]);
        ui_widget_append(w[12], w[14]);
        refs->nb_partis = ui_create_widget("text");
        ui_widget_add_class(refs->nb_partis, "text-headline-lg text-on-surface");
        ui_widget_set_text(refs->nb_partis, (const char*)widget_text_8);
        ui_widget_append(w[11], w[12]);
        ui_widget_append(w[11], refs->nb_partis);
        w[15] = ui_create_widget(NULL);
        ui_widget_add_class(w[15], "bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col justify-between");
        ui_widget_set_style_string(w[15], "width", "23%");
        ui_widget_set_style_string(w[15], "min-width", "170px");
        ui_widget_set_style_string(w[15], "flex-grow", "1");
        w[16] = ui_create_widget(NULL);
        ui_widget_add_class(w[16], "flex flex-row justify-between items-center mb-2");
        w[17] = ui_create_widget("text");
        ui_widget_add_class(w[17], "text-label-md text-secondary");
        ui_widget_set_text(w[17], (const char*)widget_text_9);
        w[18] = ui_create_widget("text");
        ui_widget_add_class(w[18], "fui-icon-regular text-primary");
        ui_widget_set_style_string(w[18], "font-size", "18px");
        ui_widget_set_text(w[18], (const char*)widget_text_10);
        ui_widget_append(w[16], w[17]);
        ui_widget_append(w[16], w[18]);
        refs->nb_bv = ui_create_widget("text");
        ui_widget_add_class(refs->nb_bv, "text-headline-lg text-on-surface");
        ui_widget_set_text(refs->nb_bv, (const char*)widget_text_11);
        ui_widget_append(w[15], w[16]);
        ui_widget_append(w[15], refs->nb_bv);
        w[19] = ui_create_widget(NULL);
        ui_widget_add_class(w[19], "bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col justify-between");
        ui_widget_set_style_string(w[19], "width", "23%");
        ui_widget_set_style_string(w[19], "min-width", "170px");
        ui_widget_set_style_string(w[19], "flex-grow", "1");
        w[20] = ui_create_widget(NULL);
        ui_widget_add_class(w[20], "flex flex-row justify-between items-center mb-2");
        w[21] = ui_create_widget("text");
        ui_widget_add_class(w[21], "text-label-md text-secondary");
        ui_widget_set_text(w[21], (const char*)widget_text_12);
        w[22] = ui_create_widget("text");
        ui_widget_add_class(w[22], "fui-icon-regular text-primary");
        ui_widget_set_style_string(w[22], "font-size", "18px");
        ui_widget_set_text(w[22], (const char*)widget_text_13);
        ui_widget_append(w[20], w[21]);
        ui_widget_append(w[20], w[22]);
        refs->nb_votes = ui_create_widget("text");
        ui_widget_add_class(refs->nb_votes, "text-headline-lg text-on-surface");
        ui_widget_set_text(refs->nb_votes, (const char*)widget_text_14);
        refs->participation = ui_create_widget("text");
        ui_widget_add_class(refs->participation, "text-label-sm text-surface-tint mt-1");
        ui_widget_append(w[19], w[20]);
        ui_widget_append(w[19], refs->nb_votes);
        ui_widget_append(w[19], refs->participation);
        ui_widget_append(w[6], w[7]);
        ui_widget_append(w[6], w[11]);
        ui_widget_append(w[6], w[15]);
        ui_widget_append(w[6], w[19]);
        w[23] = ui_create_widget(NULL);
        ui_widget_add_class(w[23], "w-full flex flex-row flex-wrap");
        w[24] = ui_create_widget(NULL);
        ui_widget_add_class(w[24], "bg-surface-container-lowest border border-outline-variant rounded-xl p-6 mr-2 mb-2 flex flex-col");
        ui_widget_set_style_string(w[24], "width", "48%");
        ui_widget_set_style_string(w[24], "min-width", "300px");
        ui_widget_set_style_string(w[24], "flex-grow", "1");
        w[25] = ui_create_widget("text");
        ui_widget_add_class(w[25], "text-headline-sm text-on-surface mb-4");
        ui_widget_set_text(w[25], (const char*)widget_text_15);
        w[26] = ui_create_widget(NULL);
        ui_widget_add_class(w[26], "flex flex-row px-2 py-2 border-b border-outline-variant mb-2");
        w[27] = ui_create_widget("text");
        ui_widget_add_class(w[27], "text-label-sm text-secondary font-semibold w-1/3");
        ui_widget_set_text(w[27], (const char*)widget_text_16);
        w[28] = ui_create_widget("text");
        ui_widget_add_class(w[28], "text-label-sm text-secondary font-semibold w-1/4");
        ui_widget_set_text(w[28], (const char*)widget_text_17);
        w[29] = ui_create_widget("text");
        ui_widget_add_class(w[29], "text-label-sm text-secondary font-semibold w-1/6 text-right");
        ui_widget_set_text(w[29], (const char*)widget_text_18);
        w[30] = ui_create_widget("text");
        ui_widget_add_class(w[30], "text-label-sm text-secondary font-semibold w-1/4 text-right");
        ui_widget_set_text(w[30], (const char*)widget_text_19);
        ui_widget_append(w[26], w[27]);
        ui_widget_append(w[26], w[28]);
        ui_widget_append(w[26], w[29]);
        ui_widget_append(w[26], w[30]);
        refs->resultats = ui_create_widget(NULL);
        ui_widget_add_class(refs->resultats, "flex flex-col");
        ui_widget_append(w[24], w[25]);
        ui_widget_append(w[24], w[26]);
        ui_widget_append(w[24], refs->resultats);
        w[31] = ui_create_widget(NULL);
        ui_widget_add_class(w[31], "bg-surface-container-lowest border border-outline-variant rounded-xl p-6 mb-2 flex flex-col");
        ui_widget_set_style_string(w[31], "width", "48%");
        ui_widget_set_style_string(w[31], "min-width", "300px");
        ui_widget_set_style_string(w[31], "flex-grow", "1");
        w[32] = ui_create_widget("text");
        ui_widget_add_class(w[32], "text-headline-sm text-on-surface mb-4");
        ui_widget_set_text(w[32], (const char*)widget_text_20);
        refs->activite = ui_create_widget(NULL);
        ui_widget_add_class(refs->activite, "flex flex-col");
        ui_widget_append(w[31], w[32]);
        ui_widget_append(w[31], refs->activite);
        ui_widget_append(w[23], w[24]);
        ui_widget_append(w[23], w[31]);
        ui_widget_append(parent, w[0]);
        ui_widget_append(parent, w[6]);
        ui_widget_append(parent, w[23]);
}

static void root_page_react_update(ui_widget_t *w)
{
        root_page_react_t *_that = ui_widget_get_data(w, root_page_proto);
}

static void dashboard_refresh_click(ui_widget_t *w, ui_event_t *e, void *arg);

static void root_page_react_init_events(ui_widget_t *w)
{
        root_page_react_t *_that = ui_widget_get_data(w, root_page_proto);
        ui_widget_on(_that->refs.ref_0, "click", dashboard_refresh_click, w);
}

static void root_page_react_init(ui_widget_t *w)
{
        root_page_react_t *_that = ui_widget_get_data(w, root_page_proto);
        root_page_load_template(w, &_that->refs);
        root_page_react_init_events(w);
}

static void root_page_react_destroy(ui_widget_t *w)
{
}

void ui_load_root_page_resources(void)
{
}
