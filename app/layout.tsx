import { RouterLink, PropsWithChildren, Text, ScrollArea, ScrollAreaContent, Scrollbar } from "@lcui/react";
import {
  Grid,
  Flag,
  PersonBoard,
  Person,
  BuildingGovernment,
  Vote,
  DataBarVertical,
} from "@lcui/react-icons";
import "@lcui/react-icons/dist/style.css";
import "./global.css";

export default function RootLayout({ children }: PropsWithChildren) {
  const menu = [
    { label: "Dashboard", to: "/", icon: <Grid fontSize={18} />, exact: true },
    { label: "Partis Politiques", to: "/partis", icon: <Flag fontSize={18} />, exact: false },
    { label: "Candidats", to: "/candidats", icon: <PersonBoard fontSize={18} />, exact: false },
    { label: "Électeurs", to: "/electeurs", icon: <Person fontSize={18} />, exact: false },
    { label: "Bureaux de Vote", to: "/bureaux", icon: <BuildingGovernment fontSize={18} />, exact: false },
    { label: "Enregistrer un Vote", to: "/vote", icon: <Vote fontSize={18} />, exact: false },
    { label: "Résultats", to: "/resultats", icon: <DataBarVertical fontSize={18} />, exact: false },
  ];
  return (
    <div className="w-full h-full flex flex-row bg-background">
      {/* SideNavBar : largeur proportionnelle, bornee */}
      <div
        className="h-full bg-surface-container border-r border-outline-variant flex flex-col py-6"
        style={{ width: "24%", minWidth: "180px", maxWidth: "270px" }}
      >
        <div className="px-4 mb-6 flex flex-row items-center">
          <div className="rounded-full bg-primary flex items-center justify-center" style={{ width: "40px", height: "40px" }}>
            <Text className="text-on-primary font-bold">CE</Text>
          </div>
          <div className="ml-2 flex flex-col">
            <Text className="text-headline-sm font-bold text-on-surface">CEP Management</Text>
            <Text className="text-label-md text-secondary">Autorité Électorale</Text>
          </div>
        </div>
        {/* Menu deroulant verticalement si la fenetre est petite */}
        <ScrollArea className="flex-1 w-full" style={{ minHeight: "0px" }}>
          <ScrollAreaContent className="w-full">
            <div className="flex flex-col px-2">
              {menu.map((item) => (
                <RouterLink
                  key={item.to}
                  to={item.to}
                  exact={item.exact}
                  className="flex flex-row items-center px-4 py-2 mb-1 rounded-lg text-secondary"
                  activeClass="bg-surface-container-high text-primary font-bold"
                >
                  {item.icon}
                  <Text className="text-label-md ml-2">{item.label}</Text>
                </RouterLink>
              ))}
            </div>
          </ScrollAreaContent>
          <Scrollbar />
        </ScrollArea>
        <div className="px-4 mt-4 pt-4 border-t border-outline-variant">
          <Text className="text-label-sm text-secondary">Élections 2026 — Haïti</Text>
        </div>
      </div>
      {/* Contenu principal */}
      <div className="flex-1 flex flex-col h-full" style={{ minWidth: "0px" }}>
        {/* TopAppBar */}
        <div className="flex flex-row flex-wrap justify-between items-center px-6 bg-surface border-b border-outline-variant" style={{ minHeight: "64px" }}>
          <Text className="text-headline-md text-primary font-semibold">
            Electoral Management System
          </Text>
          <Text className="text-label-md text-secondary">Conseil Électoral Provisoire</Text>
        </div>
        {/* Zone de contenu deroulante */}
        <ScrollArea className="flex-1 w-full" style={{ minHeight: "0px" }}>
          <ScrollAreaContent className="w-full">
            {children}
          </ScrollAreaContent>
          <Scrollbar />
        </ScrollArea>
      </div>
    </div>
  );
}
