/** This file is generated from page.tsx */
#include <ui.h>

// UTF-8 encoded string from: R%C3%A9sultats%20du%20Scrutin
static const unsigned char widget_text_0[] = {0x52, 0xc3, 0xa9, 0x73, 0x75, 0x6c, 0x74, 0x61, 0x74, 0x73, 0x20, 0x64, 0x75, 0x20, 0x53, 0x63, 0x72, 0x75, 0x74, 0x69, 0x6e, 0};
// UTF-8 encoded string from: R%C3%A9sultats%20provisoires%20%3A%20r%C3%A9sum%C3%A9%20national%20et%20d%C3%A9tail%20par%20bureau%20de%20vote.
static const unsigned char widget_text_1[] = {0x52, 0xc3, 0xa9, 0x73, 0x75, 0x6c, 0x74, 0x61, 0x74, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x76, 0x69, 0x73, 0x6f, 0x69, 0x72, 0x65, 0x73, 0x20, 0x3a, 0x20, 0x72, 0xc3, 0xa9, 0x73, 0x75, 0x6d, 0xc3, 0xa9, 0x20, 0x6e, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x61, 0x6c, 0x20, 0x65, 0x74, 0x20, 0x64, 0xc3, 0xa9, 0x74, 0x61, 0x69, 0x6c, 0x20, 0x70, 0x61, 0x72, 0x20, 0x62, 0x75, 0x72, 0x65, 0x61, 0x75, 0x20, 0x64, 0x65, 0x20, 0x76, 0x6f, 0x74, 0x65, 0x2e, 0};
// UTF-8 encoded string from: Actualiser
static const unsigned char widget_text_2[] = {0x41, 0x63, 0x74, 0x75, 0x61, 0x6c, 0x69, 0x73, 0x65, 0x72, 0};
// UTF-8 encoded string from: Bulletins
static const unsigned char widget_text_3[] = {0x42, 0x75, 0x6c, 0x6c, 0x65, 0x74, 0x69, 0x6e, 0x73, 0};
// UTF-8 encoded string from: %EE%A6%9F
static const unsigned char widget_text_4[] = {0xee, 0xa6, 0x9f, 0};
// UTF-8 encoded string from: 0
static const unsigned char widget_text_5[] = {0x30, 0};
// UTF-8 encoded string from: Votes%20exprim%C3%A9s
static const unsigned char widget_text_6[] = {0x56, 0x6f, 0x74, 0x65, 0x73, 0x20, 0x65, 0x78, 0x70, 0x72, 0x69, 0x6d, 0xc3, 0xa9, 0x73, 0};
// UTF-8 encoded string from: %EF%8C%BF
static const unsigned char widget_text_7[] = {0xef, 0x8c, 0xbf, 0};
// UTF-8 encoded string from: 0
static const unsigned char widget_text_8[] = {0x30, 0};
// UTF-8 encoded string from: Votes%20blancs
static const unsigned char widget_text_9[] = {0x56, 0x6f, 0x74, 0x65, 0x73, 0x20, 0x62, 0x6c, 0x61, 0x6e, 0x63, 0x73, 0};
// UTF-8 encoded string from: %EF%8C%BF
static const unsigned char widget_text_10[] = {0xef, 0x8c, 0xbf, 0};
// UTF-8 encoded string from: 0
static const unsigned char widget_text_11[] = {0x30, 0};
// UTF-8 encoded string from: Participation
static const unsigned char widget_text_12[] = {0x50, 0x61, 0x72, 0x74, 0x69, 0x63, 0x69, 0x70, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0};
// UTF-8 encoded string from: %EF%96%A8
static const unsigned char widget_text_13[] = {0xef, 0x96, 0xa8, 0};
// UTF-8 encoded string from: %E2%80%94
static const unsigned char widget_text_14[] = {0xe2, 0x80, 0x94, 0};
// UTF-8 encoded string from: Classement%20national%20des%20candidats
static const unsigned char widget_text_15[] = {0x43, 0x6c, 0x61, 0x73, 0x73, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x20, 0x6e, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x61, 0x6c, 0x20, 0x64, 0x65, 0x73, 0x20, 0x63, 0x61, 0x6e, 0x64, 0x69, 0x64, 0x61, 0x74, 0x73, 0};
// UTF-8 encoded string from: %EE%88%9C
static const unsigned char widget_text_16[] = {0xee, 0x88, 0x9c, 0};
// UTF-8 encoded string from: D%C3%A9tail%20par%20bureau%20de%20vote
static const unsigned char widget_text_17[] = {0x44, 0xc3, 0xa9, 0x74, 0x61, 0x69, 0x6c, 0x20, 0x70, 0x61, 0x72, 0x20, 0x62, 0x75, 0x72, 0x65, 0x61, 0x75, 0x20, 0x64, 0x65, 0x20, 0x76, 0x6f, 0x74, 0x65, 0};

typedef struct {
        ui_widget_t *ref_0;
        ui_widget_t *total_votes;
        ui_widget_t *total_exprimes;
        ui_widget_t *total_blancs;
        ui_widget_t *participation;
        ui_widget_t *national;
        ui_widget_t *bureaux;
} resultats_page_refs_t;

typedef struct resultats_page_react_state {
        char empty;
} resultats_page_react_state_t;

typedef struct resultats_page_react {
        resultats_page_react_state_t state;
        resultats_page_refs_t refs;
} resultats_page_react_t;

static ui_widget_prototype_t *resultats_page_proto;

static void resultats_page_init_prototype(void)
{
        resultats_page_proto = ui_create_widget_prototype("resultats_page", NULL);
}

static void resultats_page_load_template(ui_widget_t *parent, resultats_page_refs_t *refs)
{
        ui_widget_t *w[28];

        ui_widget_add_class(parent, "w-full p-6 flex flex-col bg-background");
        w[0] = ui_create_widget(NULL);
        ui_widget_add_class(w[0], "flex flex-row flex-wrap justify-between items-center mb-6");
        w[1] = ui_create_widget(NULL);
        ui_widget_add_class(w[1], "flex flex-col mr-4");
        w[2] = ui_create_widget("text");
        ui_widget_add_class(w[2], "text-headline-lg text-on-surface");
        ui_widget_set_text(w[2], (const char*)widget_text_0);
        w[3] = ui_create_widget("text");
        ui_widget_add_class(w[3], "text-body-md text-secondary mt-1");
        ui_widget_set_text(w[3], (const char*)widget_text_1);
        ui_widget_append(w[1], w[2]);
        ui_widget_append(w[1], w[3]);
        refs->ref_0 = ui_create_widget("button");
        ui_widget_add_class(refs->ref_0, "bg-primary rounded-lg px-4 py-2 mt-2");
        w[4] = ui_create_widget("text");
        ui_widget_add_class(w[4], "text-label-md text-on-primary font-semibold");
        ui_widget_set_text(w[4], (const char*)widget_text_2);
        ui_widget_append(refs->ref_0, w[4]);
        ui_widget_append(w[0], w[1]);
        ui_widget_append(w[0], refs->ref_0);
        w[5] = ui_create_widget(NULL);
        ui_widget_add_class(w[5], "flex flex-row flex-wrap mb-6");
        w[6] = ui_create_widget(NULL);
        ui_widget_add_class(w[6], "flex-1 bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col");
        ui_widget_set_style_string(w[6], "min-width", "150px");
        w[7] = ui_create_widget(NULL);
        ui_widget_add_class(w[7], "flex flex-row justify-between items-center mb-2");
        w[8] = ui_create_widget("text");
        ui_widget_add_class(w[8], "text-label-md text-secondary");
        ui_widget_set_text(w[8], (const char*)widget_text_3);
        w[9] = ui_create_widget("text");
        ui_widget_add_class(w[9], "fui-icon-regular text-primary");
        ui_widget_set_style_string(w[9], "font-size", "18px");
        ui_widget_set_text(w[9], (const char*)widget_text_4);
        ui_widget_append(w[7], w[8]);
        ui_widget_append(w[7], w[9]);
        refs->total_votes = ui_create_widget("text");
        ui_widget_add_class(refs->total_votes, "text-headline-lg text-on-surface");
        ui_widget_set_text(refs->total_votes, (const char*)widget_text_5);
        ui_widget_append(w[6], w[7]);
        ui_widget_append(w[6], refs->total_votes);
        w[10] = ui_create_widget(NULL);
        ui_widget_add_class(w[10], "flex-1 bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col");
        ui_widget_set_style_string(w[10], "min-width", "150px");
        w[11] = ui_create_widget(NULL);
        ui_widget_add_class(w[11], "flex flex-row justify-between items-center mb-2");
        w[12] = ui_create_widget("text");
        ui_widget_add_class(w[12], "text-label-md text-secondary");
        ui_widget_set_text(w[12], (const char*)widget_text_6);
        w[13] = ui_create_widget("text");
        ui_widget_add_class(w[13], "fui-icon-regular text-primary");
        ui_widget_set_style_string(w[13], "font-size", "18px");
        ui_widget_set_text(w[13], (const char*)widget_text_7);
        ui_widget_append(w[11], w[12]);
        ui_widget_append(w[11], w[13]);
        refs->total_exprimes = ui_create_widget("text");
        ui_widget_add_class(refs->total_exprimes, "text-headline-lg text-on-surface");
        ui_widget_set_text(refs->total_exprimes, (const char*)widget_text_8);
        ui_widget_append(w[10], w[11]);
        ui_widget_append(w[10], refs->total_exprimes);
        w[14] = ui_create_widget(NULL);
        ui_widget_add_class(w[14], "flex-1 bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col");
        ui_widget_set_style_string(w[14], "min-width", "150px");
        w[15] = ui_create_widget(NULL);
        ui_widget_add_class(w[15], "flex flex-row justify-between items-center mb-2");
        w[16] = ui_create_widget("text");
        ui_widget_add_class(w[16], "text-label-md text-secondary");
        ui_widget_set_text(w[16], (const char*)widget_text_9);
        w[17] = ui_create_widget("text");
        ui_widget_add_class(w[17], "fui-icon-regular text-secondary");
        ui_widget_set_style_string(w[17], "font-size", "18px");
        ui_widget_set_text(w[17], (const char*)widget_text_10);
        ui_widget_append(w[15], w[16]);
        ui_widget_append(w[15], w[17]);
        refs->total_blancs = ui_create_widget("text");
        ui_widget_add_class(refs->total_blancs, "text-headline-lg text-on-surface");
        ui_widget_set_text(refs->total_blancs, (const char*)widget_text_11);
        ui_widget_append(w[14], w[15]);
        ui_widget_append(w[14], refs->total_blancs);
        w[18] = ui_create_widget(NULL);
        ui_widget_add_class(w[18], "flex-1 bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mb-2 flex flex-col");
        ui_widget_set_style_string(w[18], "min-width", "150px");
        w[19] = ui_create_widget(NULL);
        ui_widget_add_class(w[19], "flex flex-row justify-between items-center mb-2");
        w[20] = ui_create_widget("text");
        ui_widget_add_class(w[20], "text-label-md text-secondary");
        ui_widget_set_text(w[20], (const char*)widget_text_12);
        w[21] = ui_create_widget("text");
        ui_widget_add_class(w[21], "fui-icon-regular text-primary");
        ui_widget_set_style_string(w[21], "font-size", "18px");
        ui_widget_set_text(w[21], (const char*)widget_text_13);
        ui_widget_append(w[19], w[20]);
        ui_widget_append(w[19], w[21]);
        refs->participation = ui_create_widget("text");
        ui_widget_add_class(refs->participation, "text-headline-lg text-on-surface");
        ui_widget_set_text(refs->participation, (const char*)widget_text_14);
        ui_widget_append(w[18], w[19]);
        ui_widget_append(w[18], refs->participation);
        ui_widget_append(w[5], w[6]);
        ui_widget_append(w[5], w[10]);
        ui_widget_append(w[5], w[14]);
        ui_widget_append(w[5], w[18]);
        w[22] = ui_create_widget(NULL);
        ui_widget_add_class(w[22], "bg-surface-container-lowest border border-outline-variant rounded-xl p-6 mb-4 flex flex-col");
        w[23] = ui_create_widget("text");
        ui_widget_add_class(w[23], "text-headline-sm text-on-surface mb-4");
        ui_widget_set_text(w[23], (const char*)widget_text_15);
        refs->national = ui_create_widget(NULL);
        ui_widget_add_class(refs->national, "flex flex-col");
        ui_widget_append(w[22], w[23]);
        ui_widget_append(w[22], refs->national);
        w[24] = ui_create_widget(NULL);
        ui_widget_add_class(w[24], "bg-surface-container-lowest border border-outline-variant rounded-xl p-6 flex flex-col");
        w[25] = ui_create_widget(NULL);
        ui_widget_add_class(w[25], "flex flex-row items-center mb-4");
        w[26] = ui_create_widget("text");
        ui_widget_add_class(w[26], "fui-icon-regular text-primary mr-2");
        ui_widget_set_style_string(w[26], "font-size", "18px");
        ui_widget_set_text(w[26], (const char*)widget_text_16);
        w[27] = ui_create_widget("text");
        ui_widget_add_class(w[27], "text-headline-sm text-on-surface");
        ui_widget_set_text(w[27], (const char*)widget_text_17);
        ui_widget_append(w[25], w[26]);
        ui_widget_append(w[25], w[27]);
        refs->bureaux = ui_create_widget(NULL);
        ui_widget_add_class(refs->bureaux, "flex flex-col");
        ui_widget_append(w[24], w[25]);
        ui_widget_append(w[24], refs->bureaux);
        ui_widget_append(parent, w[0]);
        ui_widget_append(parent, w[5]);
        ui_widget_append(parent, w[22]);
        ui_widget_append(parent, w[24]);
}

static void resultats_page_react_update(ui_widget_t *w)
{
        resultats_page_react_t *_that = ui_widget_get_data(w, resultats_page_proto);
}

static void res_refresh_click(ui_widget_t *w, ui_event_t *e, void *arg);

static void resultats_page_react_init_events(ui_widget_t *w)
{
        resultats_page_react_t *_that = ui_widget_get_data(w, resultats_page_proto);
        ui_widget_on(_that->refs.ref_0, "click", res_refresh_click, w);
}

static void resultats_page_react_init(ui_widget_t *w)
{
        resultats_page_react_t *_that = ui_widget_get_data(w, resultats_page_proto);
        resultats_page_load_template(w, &_that->refs);
        resultats_page_react_init_events(w);
}

static void resultats_page_react_destroy(ui_widget_t *w)
{
}

void ui_load_resultats_page_resources(void)
{
}
