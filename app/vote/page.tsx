import { Text, TextInput, Button } from "@lcui/react";
import { CheckmarkCircle, LockClosed, ContactCard } from "@lcui/react-icons";

export default function VotePage() {
  return (
    <div className="w-full p-6 flex flex-col items-center bg-background">
      <div className="flex flex-col items-center mb-6">
        <Text className="text-headline-lg text-on-surface">Enregistrer un Vote</Text>
        <Text className="text-body-sm text-secondary mt-1">
          Saisie sécurisée des bulletins de vote validés.
        </Text>
      </div>

      <div className="bg-surface-container-lowest rounded-xl border border-outline-variant p-6 flex flex-col w-full" style={{ maxWidth: "520px" }}>
        {/* NINU + verification */}
        <div className="flex flex-col mb-4">
          <Text className="text-body-sm font-medium text-on-surface mb-1">
            Numéro d'Identification (NINU)
          </Text>
          <div className="flex flex-row items-center">
            <TextInput $ref="ninu" className="flex-1 mr-2" placeholder="Ex: 0123456789" />
            <Button
              className="bg-surface-variant border border-outline-variant rounded px-4 py-2"
              onClick="vote_verifier_click"
            >
              <Text className="text-label-md text-on-surface font-medium">Vérifier</Text>
            </Button>
          </div>
          <Text $ref="verif_msg" className="text-body-sm mt-1" />
        </div>

        {/* Electeur / BV (lecture seule) */}
        <div className="flex flex-row flex-wrap mb-2">
          <div className="flex-1 flex flex-col mr-2 mb-2" style={{ minWidth: "180px" }}>
            <Text className="text-body-sm text-secondary mb-1">Électeur</Text>
            <div className="bg-surface-container-low border border-outline-variant rounded px-3 py-2">
              <Text $ref="electeur_nom" className="text-body-md text-secondary">En attente de NINU...</Text>
            </div>
          </div>
          <div className="flex flex-col w-40">
            <Text className="text-body-sm text-secondary mb-1">Bureau de Vote</Text>
            <div className="bg-surface-container-low border border-outline-variant rounded px-3 py-2">
              <Text $ref="electeur_bv" className="text-body-md text-secondary">—</Text>
            </div>
          </div>
        </div>

        <div className="border-b border-outline-variant mt-2 mb-4" />

        {/* Selection du candidat */}
        <Text className="text-body-sm font-medium text-on-surface mb-2">
          Sélection du Candidat
        </Text>
        <div $ref="candidats" className="flex flex-col mb-4" />

        {/* Actions */}
        <div className="flex flex-row items-center">
          <Button
            className="flex-1 bg-surface border border-outline-variant rounded px-4 py-2 mr-2"
            onClick="vote_annuler_click"
          >
            <Text className="text-label-md text-secondary font-medium">Annuler</Text>
          </Button>
          <Button
            className="flex-1 bg-primary rounded px-4 py-2"
            onClick="vote_valider_click"
          >
            <Text className="text-label-md text-on-primary font-medium">Valider le vote</Text>
          </Button>
        </div>
        <Text $ref="vote_msg" className="text-body-sm mt-2 text-center" />
      </div>

      <div className="flex flex-row items-center mt-4">
        <LockClosed fontSize={12} className="text-secondary mr-1" />
        <Text className="text-label-sm text-secondary">
          Saisie sécurisée et journalisée.
        </Text>
      </div>
    </div>
  );
}
