import { Text, TextInput, Button } from "@lcui/react";
import { Search, PersonAdd, Dismiss } from "@lcui/react-icons";

export default function ElecteursPage() {
  return (
    <div className="w-full p-6 flex flex-col bg-background">
      {/* Titre */}
      <div className="flex flex-col mb-6">
        <Text className="text-headline-lg text-on-surface">Gestion des Électeurs</Text>
        <Text className="text-body-md text-secondary mt-1">
          Gérez le registre électoral, vérifiez les statuts et ajoutez de nouveaux inscrits.
        </Text>
      </div>

      {/* Filtres */}
      <div className="bg-surface-container-lowest p-4 rounded-xl border border-outline-variant mb-4 flex flex-row flex-wrap items-center">
        <div className="flex-1 flex flex-col mr-4" style={{ minWidth: "220px" }}>
          <Text className="text-body-sm text-secondary mb-1">
            Numéro d'Identification Nationale (NINU) ou nom
          </Text>
          <TextInput
            $ref="recherche"
            placeholder="Ex: 1234567890"
            onChange="elect_search_change"
          />
        </div>
        <Button
          className="bg-primary rounded-lg px-4 py-2 mt-2"
          onClick="elect_toggle_form"
        >
          <Text className="text-label-md text-on-primary font-semibold">+ Inscrire un électeur</Text>
        </Button>
      </div>

      {/* Formulaire d'inscription / modification (repliable) */}
      <div $ref="form_panel" className="hidden bg-surface-container-lowest border border-outline-variant rounded-xl p-6 mb-4 flex-col">
        <div className="flex flex-row justify-between items-center mb-4">
          <Text $ref="form_title" className="text-headline-sm text-on-surface">Inscrire un électeur</Text>
          <Button className="px-2 py-1" onClick="elect_toggle_form">
            <Dismiss fontSize={14} />
          </Button>
        </div>
        <div className="flex flex-row flex-wrap mb-1">
          <div className="flex-1 flex flex-col mr-2 mb-2" style={{ minWidth: "150px" }}>
            <Text className="text-body-sm text-secondary mb-1">NINU (chiffres uniquement) *</Text>
            <TextInput $ref="f_ninu" placeholder="0123456789" />
          </div>
          <div className="flex-1 flex flex-col mr-2 mb-2" style={{ minWidth: "150px" }}>
            <Text className="text-body-sm text-secondary mb-1">Nom *</Text>
            <TextInput $ref="f_nom" placeholder="Nom de famille" />
          </div>
          <div className="flex-1 flex flex-col mb-2" style={{ minWidth: "150px" }}>
            <Text className="text-body-sm text-secondary mb-1">Prénom *</Text>
            <TextInput $ref="f_prenom" placeholder="Prénom" />
          </div>
        </div>
        <div className="flex flex-row flex-wrap mb-2">
          <div className="flex-1 flex flex-col mr-2 mb-2" style={{ minWidth: "220px" }}>
            <Text className="text-body-sm text-secondary mb-1">Adresse (No Rue, Rue, Département)</Text>
            <TextInput $ref="f_adresse" placeholder="5, Rue Lamarre, Ouest" />
          </div>
          <div className="flex flex-col mb-2" style={{ width: "180px" }}>
            <Text className="text-body-sm text-secondary mb-1">Bureau de Vote (ID_BV) *</Text>
            <TextInput $ref="f_bv" placeholder="BV-01" />
          </div>
        </div>
        <div className="flex flex-row items-center">
          <Button
            className="bg-primary rounded-lg px-4 py-2 mr-2"
            onClick="elect_save_click"
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
          onClick="elect_del_confirm"
        >
          <Text className="text-label-md text-on-primary font-semibold">Confirmer</Text>
        </Button>
        <Button
          className="bg-surface border border-outline-variant rounded-lg px-4 py-2"
          onClick="elect_del_cancel"
        >
          <Text className="text-label-md text-secondary font-semibold">Annuler</Text>
        </Button>
      </div>

      {/* Table des electeurs */}
      <div className="bg-surface-container-lowest border border-outline-variant rounded-xl flex flex-col">
        <div className="flex flex-row items-center w-full border-b border-outline-variant px-4 py-3 bg-surface-container-low">
          <Text className="text-label-sm text-secondary font-semibold w-40">NINU</Text>
          <Text className="text-label-sm text-secondary font-semibold flex-1">Nom &amp; Prénom</Text>
          <Text className="text-label-sm text-secondary font-semibold w-32">Département</Text>
          <Text className="text-label-sm text-secondary font-semibold w-32">Bureau de Vote</Text>
          <Text className="text-label-sm text-secondary font-semibold w-28 text-center">Statut du Vote</Text>
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
