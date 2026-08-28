import { Text, Button } from "@lcui/react";
import { ArrowSync, DataBarVertical, BuildingGovernment, Poll, People } from "@lcui/react-icons";

export default function ResultatsPage() {
  return (
    <div className="w-full p-6 flex flex-col bg-background">
      {/* Titre */}
      <div className="flex flex-row flex-wrap justify-between items-center mb-6">
        <div className="flex flex-col mr-4">
          <Text className="text-headline-lg text-on-surface">Résultats du Scrutin</Text>
          <Text className="text-body-md text-secondary mt-1">
            Résultats provisoires : résumé national et détail par bureau de vote.
          </Text>
        </div>
        <Button
          className="bg-primary rounded-lg px-4 py-2 mt-2"
          onClick="res_refresh_click"
        >
          <Text className="text-label-md text-on-primary font-semibold">Actualiser</Text>
        </Button>
      </div>

      {/* Synthese (4 cartes) */}
      <div className="flex flex-row flex-wrap mb-6">
        <div className="flex-1 bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col" style={{ minWidth: "150px" }}>
          <div className="flex flex-row justify-between items-center mb-2">
            <Text className="text-label-md text-secondary">Bulletins</Text>
            <Poll fontSize={18} className="text-primary" />
          </div>
          <Text $ref="total_votes" className="text-headline-lg text-on-surface">0</Text>
        </div>
        <div className="flex-1 bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col" style={{ minWidth: "150px" }}>
          <div className="flex flex-row justify-between items-center mb-2">
            <Text className="text-label-md text-secondary">Votes exprimés</Text>
            <DataBarVertical fontSize={18} className="text-primary" />
          </div>
          <Text $ref="total_exprimes" className="text-headline-lg text-on-surface">0</Text>
        </div>
        <div className="flex-1 bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col" style={{ minWidth: "150px" }}>
          <div className="flex flex-row justify-between items-center mb-2">
            <Text className="text-label-md text-secondary">Votes blancs</Text>
            <DataBarVertical fontSize={18} className="text-secondary" />
          </div>
          <Text $ref="total_blancs" className="text-headline-lg text-on-surface">0</Text>
        </div>
        <div className="flex-1 bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mb-2 flex flex-col" style={{ minWidth: "150px" }}>
          <div className="flex flex-row justify-between items-center mb-2">
            <Text className="text-label-md text-secondary">Participation</Text>
            <People fontSize={18} className="text-primary" />
          </div>
          <Text $ref="participation" className="text-headline-lg text-on-surface">—</Text>
        </div>
      </div>

      {/* Resultats nationaux avec barres */}
      <div className="bg-surface-container-lowest border border-outline-variant rounded-xl p-6 mb-4 flex flex-col">
        <Text className="text-headline-sm text-on-surface mb-4">
          Classement national des candidats
        </Text>
        <div $ref="national" className="flex flex-col" />
      </div>

      {/* Detail par bureau de vote */}
      <div className="bg-surface-container-lowest border border-outline-variant rounded-xl p-6 flex flex-col">
        <div className="flex flex-row items-center mb-4">
          <BuildingGovernment fontSize={18} className="text-primary mr-2" />
          <Text className="text-headline-sm text-on-surface">
            Détail par bureau de vote
          </Text>
        </div>
        <div $ref="bureaux" className="flex flex-col" />
      </div>
    </div>
  );
}
