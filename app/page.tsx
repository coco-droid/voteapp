import { Text, Button } from "@lcui/react";
import {
  People,
  Flag,
  BuildingGovernment,
  Poll,
  ArrowSync,
} from "@lcui/react-icons";

export default function Dashboard() {
  return (
    <div className="w-full p-6 flex flex-col bg-background">
      {/* Titre de page */}
      <div className="flex flex-row justify-between items-center mb-6">
        <div className="flex flex-col">
          <Text className="text-headline-lg text-on-surface">Tableau de bord</Text>
          <Text className="text-body-sm text-secondary mt-1">
            Aperçu général du système électoral.
          </Text>
        </div>
        <div className="flex flex-row items-center">
          <Button
            className="bg-surface border border-outline-variant rounded-lg px-3 py-1 mr-2"
            onClick="dashboard_refresh_click"
          >
            <ArrowSync fontSize={14} />
          </Button>
          <Text $ref="date_maj" className="text-label-sm text-secondary" />
        </div>
      </div>

      {/* Grille de metriques (4 cartes, repliees en 2x2 sur petit ecran) */}
      <div className="w-full flex flex-row flex-wrap mb-4">
        <div className="bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col justify-between" style={{ width: "23%", minWidth: "170px", flexGrow: 1 }}>
          <div className="flex flex-row justify-between items-center mb-2">
            <Text className="text-label-md text-secondary">Total Électeurs</Text>
            <People fontSize={18} className="text-primary" />
          </div>
          <Text $ref="nb_electeurs" className="text-headline-lg text-on-surface">0</Text>
        </div>
        <div className="bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col justify-between" style={{ width: "23%", minWidth: "170px", flexGrow: 1 }}>
          <div className="flex flex-row justify-between items-center mb-2">
            <Text className="text-label-md text-secondary">Partis Inscrits</Text>
            <Flag fontSize={18} className="text-primary" />
          </div>
          <Text $ref="nb_partis" className="text-headline-lg text-on-surface">0</Text>
        </div>
        <div className="bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col justify-between" style={{ width: "23%", minWidth: "170px", flexGrow: 1 }}>
          <div className="flex flex-row justify-between items-center mb-2">
            <Text className="text-label-md text-secondary">Bureaux de Vote</Text>
            <BuildingGovernment fontSize={18} className="text-primary" />
          </div>
          <Text $ref="nb_bv" className="text-headline-lg text-on-surface">0</Text>
        </div>
        <div className="bg-surface-container-lowest border border-outline-variant rounded-xl p-4 mr-2 mb-2 flex flex-col justify-between" style={{ width: "23%", minWidth: "170px", flexGrow: 1 }}>
          <div className="flex flex-row justify-between items-center mb-2">
            <Text className="text-label-md text-secondary">Votes Enregistrés</Text>
            <Poll fontSize={18} className="text-primary" />
          </div>
          <Text $ref="nb_votes" className="text-headline-lg text-on-surface">0</Text>
          <Text $ref="participation" className="text-label-sm text-surface-tint mt-1" />
        </div>
      </div>

      {/* Section inferieure : resultats nationaux + activite (empilees sur petit ecran) */}
      <div className="w-full flex flex-row flex-wrap">
        {/* Resultats nationaux provisoires */}
        <div className="bg-surface-container-lowest border border-outline-variant rounded-xl p-6 mr-2 mb-2 flex flex-col" style={{ width: "48%", minWidth: "300px", flexGrow: 1 }}>
          <Text className="text-headline-sm text-on-surface mb-4">
            Résultats nationaux (provisoires)
          </Text>
          <div className="flex flex-row px-2 py-2 border-b border-outline-variant mb-2">
            <Text className="text-label-sm text-secondary font-semibold w-1/3">Candidat</Text>
            <Text className="text-label-sm text-secondary font-semibold w-1/4">Parti / Type</Text>
            <Text className="text-label-sm text-secondary font-semibold w-1/6 text-right">Voix</Text>
            <Text className="text-label-sm text-secondary font-semibold w-1/4 text-right">%</Text>
          </div>
          <div $ref="resultats" className="flex flex-col" />
        </div>

        {/* Activite recente */}
        <div className="bg-surface-container-lowest border border-outline-variant rounded-xl p-6 mb-2 flex flex-col" style={{ width: "48%", minWidth: "300px", flexGrow: 1 }}>
          <Text className="text-headline-sm text-on-surface mb-4">Activité Récente</Text>
          <div $ref="activite" className="flex flex-col" />
        </div>
      </div>
    </div>
  );
}
