import { Text, TextInput, Button } from "@lcui/react";
import { Search, Dismiss } from "@lcui/react-icons";

export default function CandidatsPage() {
  return (
    <div className="w-full p-6 flex flex-col bg-background">
      {/* Titre */}
      <div className="flex flex-col mb-6">
        <Text className="text-headline-lg text-on-surface">Gestion des Candidats</Text>
        <Text className="text-body-md text-secondary mt-1">
          Enregistrez les candidats et leur affiliation politique (ou indépendants).
        </Text>
      </div>

      {/* Barre d'outils */}
      <div className="bg-surface-container-lowest border border-outline-variant rounded-lg p-4 mb-4 flex flex-row flex-wrap items-center justify-between">
        <div className="flex flex-row items-center flex-1" style={{ minWidth: "200px" }}>
          <Search fontSize={16} className="text-secondary mr-2" />
          <TextInput
            $ref="recherche"
            className="w-full"
            placeholder="Rechercher par nom ou ID..."
            onChange="cand_search_change"
          />
        </div>
        <Button
          className="bg-primary rounded-lg px-4 py-2 ml-2 mt-2"
          onClick="cand_toggle_form"
        >
          <Text className="text-label-md text-on-primary font-semibold">+ Nouveau candidat</Text>
        </Button>
      </div>

      {/* Formulaire d'ajout / modification (repliable) */}
      <div $ref="form_panel" className="hidden bg-surface-container-lowest border border-outline-variant rounded-xl p-6 mb-4 flex-col">
        <div className="flex flex-row justify-between items-center mb-4">
          <Text $ref="form_title" className="text-headline-sm text-on-surface">Nouveau candidat</Text>
          <Button className="px-2 py-1" onClick="cand_toggle_form">
            <Dismiss fontSize={14} />
          </Button>
        </div>
        <div className="flex flex-row flex-wrap mb-1">
          <div className="flex-1 flex flex-col mr-2 mb-2" style={{ minWidth: "140px" }}>
            <Text className="text-body-sm text-secondary mb-1">Identifiant (automatique)</Text>
            <TextInput $ref="f_id" placeholder="CA-XXX" />
          </div>
          <div className="flex-1 flex flex-col mr-2 mb-2" style={{ minWidth: "140px" }}>
            <Text className="text-body-sm text-secondary mb-1">Nom *</Text>
            <TextInput $ref="f_nom" placeholder="Nom de famille" />
          </div>
          <div className="flex-1 flex flex-col mb-2" style={{ minWidth: "140px" }}>
            <Text className="text-body-sm text-secondary mb-1">Prénom *</Text>
            <TextInput $ref="f_prenom" placeholder="Prénom" />
          </div>
        </div>
        <div className="flex flex-row flex-wrap mb-1">
          <div className="flex-1 flex flex-col mr-2 mb-2" style={{ minWidth: "140px" }}>
            <Text className="text-body-sm text-secondary mb-1">Date de naissance (JJ/MM/AAAA)</Text>
            <TextInput $ref="f_naissance" placeholder="01/01/1980" />
          </div>
          <div className="flex-1 flex flex-col mr-2 mb-2" style={{ minWidth: "140px" }}>
            <Text className="text-body-sm text-secondary mb-1">Téléphone</Text>
            <TextInput $ref="f_tel" placeholder="+509 ..." />
          </div>
          <div className="flex-1 flex flex-col mb-2" style={{ minWidth: "180px" }}>
            <Text className="text-body-sm text-secondary mb-1">Email</Text>
            <TextInput $ref="f_email" placeholder="candidat@mail.ht" />
          </div>
        </div>
        <div className="flex flex-row flex-wrap mb-1">
          <div className="flex-1 flex flex-col mr-2 mb-2" style={{ minWidth: "200px" }}>
            <Text className="text-body-sm text-secondary mb-1">Adresse (No Rue, Rue, Département)</Text>
            <TextInput $ref="f_adresse" placeholder="8, Rue Bonne Foi, Ouest" />
          </div>
          <div className="flex flex-col mr-2 mb-2" style={{ width: "180px" }}>
            <Text className="text-body-sm text-secondary mb-1">Parti (ID_PP) ou 0 = indép. *</Text>
            <TextInput $ref="f_type" placeholder="0" />
          </div>
        </div>
        <Text className="text-label-sm text-secondary mb-3">
          La date d'inscription est enregistrée automatiquement (date du jour).
        </Text>
        <div className="flex flex-row items-center flex-wrap">
          <Button
            className="bg-primary rounded-lg px-4 py-2 mr-2"
            onClick="cand_save_click"
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
          onClick="cand_del_confirm"
        >
          <Text className="text-label-md text-on-primary font-semibold">Confirmer</Text>
        </Button>
        <Button
          className="bg-surface border border-outline-variant rounded-lg px-4 py-2"
          onClick="cand_del_cancel"
        >
          <Text className="text-label-md text-secondary font-semibold">Annuler</Text>
        </Button>
      </div>

      {/* Table des candidats */}
      <div className="bg-surface-container-lowest border border-outline-variant rounded-xl flex flex-col">
        <div className="flex flex-row items-center w-full border-b border-outline-variant px-4 py-3 bg-surface-container-low">
          <Text className="text-label-sm text-secondary font-semibold w-24">ID</Text>
          <Text className="text-label-sm text-secondary font-semibold flex-1">Nom &amp; Prénom</Text>
          <Text className="text-label-sm text-secondary font-semibold w-28">Naissance</Text>
          <Text className="text-label-sm text-secondary font-semibold w-32">Parti</Text>
          <Text className="text-label-sm text-secondary font-semibold w-36">Contact</Text>
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
