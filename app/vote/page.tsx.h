/** This file is generated from page.tsx */
#include <ui.h>

// UTF-8 encoded string from: Enregistrer%20un%20Vote
static const unsigned char widget_text_0[] = {0x45, 0x6e, 0x72, 0x65, 0x67, 0x69, 0x73, 0x74, 0x72, 0x65, 0x72, 0x20, 0x75, 0x6e, 0x20, 0x56, 0x6f, 0x74, 0x65, 0};
// UTF-8 encoded string from: Saisie%20s%C3%A9curis%C3%A9e%20des%20bulletins%20de%20vote%20valid%C3%A9s.
static const unsigned char widget_text_1[] = {0x53, 0x61, 0x69, 0x73, 0x69, 0x65, 0x20, 0x73, 0xc3, 0xa9, 0x63, 0x75, 0x72, 0x69, 0x73, 0xc3, 0xa9, 0x65, 0x20, 0x64, 0x65, 0x73, 0x20, 0x62, 0x75, 0x6c, 0x6c, 0x65, 0x74, 0x69, 0x6e, 0x73, 0x20, 0x64, 0x65, 0x20, 0x76, 0x6f, 0x74, 0x65, 0x20, 0x76, 0x61, 0x6c, 0x69, 0x64, 0xc3, 0xa9, 0x73, 0x2e, 0};
// UTF-8 encoded string from: Num%C3%A9ro%20d'Identification%20(NINU)%20*
static const unsigned char widget_text_2[] = {0x4e, 0x75, 0x6d, 0xc3, 0xa9, 0x72, 0x6f, 0x20, 0x64, 0x27, 0x49, 0x64, 0x65, 0x6e, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x28, 0x4e, 0x49, 0x4e, 0x55, 0x29, 0x20, 0x2a, 0};
// UTF-8 encoded string from: V%C3%A9rifier%20le%20NINU
static const unsigned char widget_text_3[] = {0x56, 0xc3, 0xa9, 0x72, 0x69, 0x66, 0x69, 0x65, 0x72, 0x20, 0x6c, 0x65, 0x20, 0x4e, 0x49, 0x4e, 0x55, 0};
// UTF-8 encoded string from: %C3%89lecteur
static const unsigned char widget_text_4[] = {0xc3, 0x89, 0x6c, 0x65, 0x63, 0x74, 0x65, 0x75, 0x72, 0};
// UTF-8 encoded string from: %E2%80%94
static const unsigned char widget_text_5[] = {0xe2, 0x80, 0x94, 0};
// UTF-8 encoded string from: Bureau%20de%20vote
static const unsigned char widget_text_6[] = {0x42, 0x75, 0x72, 0x65, 0x61, 0x75, 0x20, 0x64, 0x65, 0x20, 0x76, 0x6f, 0x74, 0x65, 0};
// UTF-8 encoded string from: %E2%80%94
static const unsigned char widget_text_7[] = {0xe2, 0x80, 0x94, 0};
// UTF-8 encoded string from: S%C3%A9lection%20du%20Candidat%20*
static const unsigned char widget_text_8[] = {0x53, 0xc3, 0xa9, 0x6c, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x64, 0x75, 0x20, 0x43, 0x61, 0x6e, 0x64, 0x69, 0x64, 0x61, 0x74, 0x20, 0x2a, 0};
// UTF-8 encoded string from: Annuler
static const unsigned char widget_text_9[] = {0x41, 0x6e, 0x6e, 0x75, 0x6c, 0x65, 0x72, 0};
// UTF-8 encoded string from: Valider%20le%20vote
static const unsigned char widget_text_10[] = {0x56, 0x61, 0x6c, 0x69, 0x64, 0x65, 0x72, 0x20, 0x6c, 0x65, 0x20, 0x76, 0x6f, 0x74, 0x65, 0};
// UTF-8 encoded string from: %EE%9E%8D
static const unsigned char widget_text_11[] = {0xee, 0x9e, 0x8d, 0};
// UTF-8 encoded string from: Saisie%20s%C3%A9curis%C3%A9e%20et%20journalis%C3%A9e.
static const unsigned char widget_text_12[] = {0x53, 0x61, 0x69, 0x73, 0x69, 0x65, 0x20, 0x73, 0xc3, 0xa9, 0x63, 0x75, 0x72, 0x69, 0x73, 0xc3, 0xa9, 0x65, 0x20, 0x65, 0x74, 0x20, 0x6a, 0x6f, 0x75, 0x72, 0x6e, 0x61, 0x6c, 0x69, 0x73, 0xc3, 0xa9, 0x65, 0x2e, 0};

typedef struct {
        ui_widget_t *ninu;
        ui_widget_t *ref_1;
        ui_widget_t *verif_msg;
        ui_widget_t *electeur_nom;
        ui_widget_t *electeur_bv;
        ui_widget_t *candidats;
        ui_widget_t *ref_6;
        ui_widget_t *ref_7;
        ui_widget_t *vote_msg;
} vote_page_refs_t;

typedef struct vote_page_react_state {
        char empty;
} vote_page_react_state_t;

typedef struct vote_page_react {
        vote_page_react_state_t state;
        vote_page_refs_t refs;
} vote_page_react_t;

static ui_widget_prototype_t *vote_page_proto;

static void vote_page_init_prototype(void)
{
        vote_page_proto = ui_create_widget_prototype("vote_page", NULL);
}

static void vote_page_load_template(ui_widget_t *parent, vote_page_refs_t *refs)
{
        ui_widget_t *w[20];

        ui_widget_add_class(parent, "w-full p-6 flex flex-col items-center bg-background");
        w[0] = ui_create_widget(NULL);
        ui_widget_add_class(w[0], "flex flex-col items-center mb-6");
        w[1] = ui_create_widget("text");
        ui_widget_add_class(w[1], "text-headline-lg text-on-surface");
        ui_widget_set_text(w[1], (const char*)widget_text_0);
        w[2] = ui_create_widget("text");
        ui_widget_add_class(w[2], "text-body-sm text-secondary mt-1");
        ui_widget_set_text(w[2], (const char*)widget_text_1);
        ui_widget_append(w[0], w[1]);
        ui_widget_append(w[0], w[2]);
        w[3] = ui_create_widget(NULL);
        ui_widget_add_class(w[3], "bg-surface-container-lowest rounded-xl border border-outline-variant p-6 flex flex-col w-full");
        ui_widget_set_style_string(w[3], "max-width", "520px");
        w[4] = ui_create_widget(NULL);
        ui_widget_add_class(w[4], "flex flex-col mb-2");
        w[5] = ui_create_widget("text");
        ui_widget_add_class(w[5], "text-body-sm font-medium text-on-surface mb-1");
        ui_widget_set_text(w[5], (const char*)widget_text_2);
        refs->ninu = ui_create_widget("textinput");
        ui_widget_add_class(refs->ninu, "w-full mb-2");
        ui_widget_set_attr(refs->ninu, "placeholder", "Ex: 0123456789");
        refs->ref_1 = ui_create_widget("button");
        ui_widget_add_class(refs->ref_1, "bg-surface-variant border border-outline-variant rounded px-4 py-2");
        w[6] = ui_create_widget("text");
        ui_widget_add_class(w[6], "text-label-md text-on-surface font-medium");
        ui_widget_set_text(w[6], (const char*)widget_text_3);
        ui_widget_append(refs->ref_1, w[6]);
        refs->verif_msg = ui_create_widget("text");
        ui_widget_add_class(refs->verif_msg, "text-body-sm mt-1");
        ui_widget_append(w[4], w[5]);
        ui_widget_append(w[4], refs->ninu);
        ui_widget_append(w[4], refs->ref_1);
        ui_widget_append(w[4], refs->verif_msg);
        w[7] = ui_create_widget(NULL);
        ui_widget_add_class(w[7], "bg-surface-container-low rounded-lg px-3 py-2 mb-2 flex flex-col");
        w[8] = ui_create_widget(NULL);
        ui_widget_add_class(w[8], "flex flex-row mb-1");
        w[9] = ui_create_widget("text");
        ui_widget_add_class(w[9], "text-body-sm text-secondary w-32");
        ui_widget_set_text(w[9], (const char*)widget_text_4);
        refs->electeur_nom = ui_create_widget("text");
        ui_widget_add_class(refs->electeur_nom, "text-body-md text-on-surface flex-1");
        ui_widget_set_text(refs->electeur_nom, (const char*)widget_text_5);
        ui_widget_append(w[8], w[9]);
        ui_widget_append(w[8], refs->electeur_nom);
        w[10] = ui_create_widget(NULL);
        ui_widget_add_class(w[10], "flex flex-row");
        w[11] = ui_create_widget("text");
        ui_widget_add_class(w[11], "text-body-sm text-secondary w-32");
        ui_widget_set_text(w[11], (const char*)widget_text_6);
        refs->electeur_bv = ui_create_widget("text");
        ui_widget_add_class(refs->electeur_bv, "text-body-md text-on-surface flex-1");
        ui_widget_set_text(refs->electeur_bv, (const char*)widget_text_7);
        ui_widget_append(w[10], w[11]);
        ui_widget_append(w[10], refs->electeur_bv);
        ui_widget_append(w[7], w[8]);
        ui_widget_append(w[7], w[10]);
        w[12] = ui_create_widget(NULL);
        ui_widget_add_class(w[12], "border-b border-outline-variant mt-2 mb-4");
        w[13] = ui_create_widget("text");
        ui_widget_add_class(w[13], "text-body-sm font-medium text-on-surface mb-2");
        ui_widget_set_text(w[13], (const char*)widget_text_8);
        refs->candidats = ui_create_widget(NULL);
        ui_widget_add_class(refs->candidats, "flex flex-col mb-4");
        w[14] = ui_create_widget(NULL);
        ui_widget_add_class(w[14], "flex flex-row items-center");
        refs->ref_6 = ui_create_widget("button");
        ui_widget_add_class(refs->ref_6, "flex-1 bg-surface border border-outline-variant rounded px-4 py-2 mr-2");
        w[15] = ui_create_widget("text");
        ui_widget_add_class(w[15], "text-label-md text-secondary font-medium");
        ui_widget_set_text(w[15], (const char*)widget_text_9);
        ui_widget_append(refs->ref_6, w[15]);
        refs->ref_7 = ui_create_widget("button");
        ui_widget_add_class(refs->ref_7, "flex-1 bg-primary rounded px-4 py-2");
        w[16] = ui_create_widget("text");
        ui_widget_add_class(w[16], "text-label-md text-on-primary font-medium");
        ui_widget_set_text(w[16], (const char*)widget_text_10);
        ui_widget_append(refs->ref_7, w[16]);
        ui_widget_append(w[14], refs->ref_6);
        ui_widget_append(w[14], refs->ref_7);
        refs->vote_msg = ui_create_widget("text");
        ui_widget_add_class(refs->vote_msg, "text-body-sm mt-2 text-center");
        ui_widget_append(w[3], w[4]);
        ui_widget_append(w[3], w[7]);
        ui_widget_append(w[3], w[12]);
        ui_widget_append(w[3], w[13]);
        ui_widget_append(w[3], refs->candidats);
        ui_widget_append(w[3], w[14]);
        ui_widget_append(w[3], refs->vote_msg);
        w[17] = ui_create_widget(NULL);
        ui_widget_add_class(w[17], "flex flex-row items-center mt-4");
        w[18] = ui_create_widget("text");
        ui_widget_add_class(w[18], "fui-icon-regular text-secondary mr-1");
        ui_widget_set_style_string(w[18], "font-size", "12px");
        ui_widget_set_text(w[18], (const char*)widget_text_11);
        w[19] = ui_create_widget("text");
        ui_widget_add_class(w[19], "text-label-sm text-secondary");
        ui_widget_set_text(w[19], (const char*)widget_text_12);
        ui_widget_append(w[17], w[18]);
        ui_widget_append(w[17], w[19]);
        ui_widget_append(parent, w[0]);
        ui_widget_append(parent, w[3]);
        ui_widget_append(parent, w[17]);
}

static void vote_page_react_update(ui_widget_t *w)
{
        vote_page_react_t *_that = ui_widget_get_data(w, vote_page_proto);
}

static void vote_verifier_click(ui_widget_t *w, ui_event_t *e, void *arg);

static void vote_annuler_click(ui_widget_t *w, ui_event_t *e, void *arg);

static void vote_valider_click(ui_widget_t *w, ui_event_t *e, void *arg);

static void vote_page_react_init_events(ui_widget_t *w)
{
        vote_page_react_t *_that = ui_widget_get_data(w, vote_page_proto);
        ui_widget_on(_that->refs.ref_1, "click", vote_verifier_click, w);
        ui_widget_on(_that->refs.ref_6, "click", vote_annuler_click, w);
        ui_widget_on(_that->refs.ref_7, "click", vote_valider_click, w);
}

static void vote_page_react_init(ui_widget_t *w)
{
        vote_page_react_t *_that = ui_widget_get_data(w, vote_page_proto);
        vote_page_load_template(w, &_that->refs);
        vote_page_react_init_events(w);
}

static void vote_page_react_destroy(ui_widget_t *w)
{
}

void ui_load_vote_page_resources(void)
{
}
