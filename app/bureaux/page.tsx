import { Text, TextInput, Button } from "@lcui/react";
import { Search, Dismiss } from "@lcui/react-icons";

export default function BureauxPage() {
  return (
    <div className="w-full p-6 flex flex-col bg-background">
      {/* Titre */}
      <div className="flex flex-col mb-6">
        <Text className="text-headline-lg text-on-surface">Bureaux de Vote</Text>
        <Text className="text-body-md text-secondary mt-1">
          Gérez les bureaux de vote et leurs membres (président, vice-président, secrétaire).
        </Text>
      </div>

      {/* Barre d'outils */}
      <div className="bg-surface-container-lowest border border-outline-variant rounded-lg p-4 mb-4 flex flex-row flex-wrap items-center justify-between">
        <div className="flex flex-row items-center flex-1" style={{ minWidth: "200px" }}>
          <Search fontSize={16} className="text-secondary mr-2" />
          <TextInput
            $ref="recherche"
            className="w-full"
            placeholder="Rechercher par ID ou adresse..."
            onChange="bv_search_change"
          />
        </div>
        <Button
          className="bg-primary rounded-lg px-4 py-2 ml-2 mt-2"
          onClick="bv_toggle_form"
        >
          <Text className="text-label-md text-on-primary font-semibold">+ Nouveau bureau</Text>
        </Button>
      </div>

      {/* Formulaire d'ajout / modification (repliable) */}
      <div $ref="form_panel" className="hidden bg-surface-container-lowest border border-outline-variant rounded-xl p-6 mb-4 flex-col">
        <div className="flex flex-row justify-between items-center mb-4">
          <Text $ref="form_title" className="text-headline-sm text-on-surface">Nouveau bureau de vote</Text>
          <Button className="px-2 py-1" onClick="bv_toggle_form">
            <Dismiss fontSize={14} />
          </Button>
        </div>
        <div className="flex flex-row flex-wrap mb-1">
          <div className="flex flex-col mr-2 mb-2" style={{ width: "160px" }}>
            <Text className="text-body-sm text-secondary mb-1">ID du bureau (automatique)</Text>
            <TextInput $ref="f_id" placeholder="BV-XX" />
          </div>
          <div className="flex-1 flex flex-col mb-2" style={{ minWidth: "220px" }}>
            <Text className="text-body-sm text-secondary mb-1">Adresse (No Rue, Rue, Département) *</Text>
            <TextInput $ref="f_adresse" placeholder="3, Rue Capois, Port-au-Prince, Ouest" />
          </div>
        </div>
        <div className="flex flex-row flex-wrap mb-2">
          <div className="flex-1 flex flex-col mr-2 mb-2" style={{ minWidth: "150px" }}>
            <Text className="text-body-sm text-secondary mb-1">ID Président *</Text>
            <TextInput $ref="f_president" placeholder="ID de la personne" />
          </div>
          <div className="flex-1 flex flex-col mr-2 mb-2" style={{ minWidth: "150px" }}>
            <Text className="text-body-sm text-secondary mb-1">ID Vice-Président *</Text>
            <TextInput $ref="f_vpresident" placeholder="ID de la personne" />
          </div>
          <div className="flex-1 flex flex-col mb-2" style={{ minWidth: "150px" }}>
            <Text className="text-body-sm text-secondary mb-1">ID Secrétaire *</Text>
            <TextInput $ref="f_secretaire" placeholder="ID de la personne" />
          </div>
        </div>
        <Text className="text-label-sm text-secondary mb-3">
          Une même personne ne peut occuper qu'un seul poste, dans un seul bureau.
        </Text>
        <div className="flex flex-row items-center flex-wrap">
          <Button
            className="bg-primary rounded-lg px-4 py-2 mr-2"
            onClick="bv_save_click"
          >
            <Text className="text-label-md text-on-primary font-semibold">Enregistrer</Text>
          </Button>
          <Text $ref="form_msg" className="text-body-sm ml-2" />
        </div>
      </div>

      {/* Barre de confirmation de suppression */}
      <div $ref="confirm_panel" className="hidden bg-error-container border border-outline-variant rounded-xl p-4 mb-4 flex-row items-center">
        <Text $ref="confirm_msg" className="text-body-md text-on-surface flex-1" />
        <Button
          className="bg-primary rounded-lg px-4 py-2 mr-2"
          onClick="bv_del_confirm"
        >
          <Text className="text-label-md text-on-primary font-semibold">Confirmer</Text>
        </Button>
        <Button
          className="bg-surface border border-outline-variant rounded-lg px-4 py-2"
          onClick="bv_del_cancel"
        >
          <Text className="text-label-md text-secondary font-semibold">Annuler</Text>
        </Button>
      </div>

      {/* Table des bureaux de vote */}
      <div className="bg-surface-container-lowest border border-outline-variant rounded-xl flex flex-col">
        <div className="flex flex-row items-center w-full border-b border-outline-variant px-4 py-3 bg-surface-container-low">
          <Text className="text-label-sm text-secondary font-semibold w-24">ID_BV</Text>
          <Text className="text-label-sm text-secondary font-semibold flex-1">Adresse</Text>
          <Text className="text-label-sm text-secondary font-semibold w-28">Département</Text>
          <Text className="text-label-sm text-secondary font-semibold w-32">Président</Text>
          <Text className="text-label-sm text-secondary font-semibold w-28 text-center">Électeurs</Text>
          <Text className="text-label-sm text-secondary font-semibold w-36 text-center">Actions</Text>
        </div>
        <div $ref="table_body" className="flex flex-col" />
        <div className="flex flex-row justify-between items-center px-4 py-3">
          <Text $ref="table_count" className="text-body-sm text-secondary" />
          <Text $ref="table_msg" className="text-body-sm" />
        </div>
      </div>
    </div>
  );
}
