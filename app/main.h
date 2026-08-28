#include <locale.h>
#include <LCUI.h>
#include <LCUI/main.h>
#include <router.h>
#include "bureaux/page.h"
#include "candidats/page.h"
#include "electeurs/page.h"
#include "layout.h"
#include "page.h"
#include "partis/page.h"
#include "resultats/page.h"
#include "vote/page.h"

static void app_router_init(void)
{
        router_config_t *config;
        router_t *router = router_create("AppRouter");

        config = router_config_create();
        router_config_set_path(config, "/bureaux");
        router_config_set_component(config, NULL, "bureaux_page");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/candidats");
        router_config_set_component(config, NULL, "candidats_page");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/electeurs");
        router_config_set_component(config, NULL, "electeurs_page");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/partis");
        router_config_set_component(config, NULL, "partis_page");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/resultats");
        router_config_set_component(config, NULL, "resultats_page");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/vote");
        router_config_set_component(config, NULL, "vote_page");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/");
        router_config_set_component(config, NULL, "root_page");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);
}

static void app_init(void)
{
        lcui_init();
        app_router_init();
        ui_load_bureaux_page_resources();
        ui_load_candidats_page_resources();
        ui_load_electeurs_page_resources();
        ui_load_root_layout_resources();
        ui_load_root_page_resources();
        ui_load_partis_page_resources();
        ui_load_resultats_page_resources();
        ui_load_vote_page_resources();
        ui_register_bureaux_page();
        ui_register_candidats_page();
        ui_register_electeurs_page();
        ui_register_root_layout();
        ui_register_root_page();
        ui_register_partis_page();
        ui_register_resultats_page();
        ui_register_vote_page();
        ui_widget_set_attr(ui_root(), "router", "AppRouter");
        ui_widget_append(ui_root(), ui_create_root_layout());
}

static int app_run(void)
{
        return lcui_run();
}
