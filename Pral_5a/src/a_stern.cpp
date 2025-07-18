#include "astern/text_visualizer.h"
#include "astern/unit.h"
#ifdef SFML_ENABLED
#include "astern/sfml_visualizer.h"
#endif
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <list>
#include <memory>
#include <limits>

// Ein Graph, der Koordinaten von Knoten speichert.
class CoordinateGraph : public DistanceGraph
{
private:
  std::vector<std::pair<double, double>> coordinates; // (lon, lat) for each vertex
  std::vector<std::vector<std::pair<VertexT, CostT>>> adjacencyList;

public:
  CoordinateGraph() = default;

  const NeighborT getNeighbors(VertexT v) const override
  {
    if (v >= vertexCount)
      return NeighborT();

    NeighborT neighbors;
    for (const auto &edge : adjacencyList[v])
    {
      neighbors.push_back({edge.first, edge.second});
    }
    return neighbors;
  }

  CostT estimatedCost(VertexT from, VertexT to) const override
  {
    if (from >= coordinates.size() || to >= coordinates.size())
    {
      return infty;
    }

    // Nutze die euklidische Distanz als Heuristik
    double dx = coordinates[from].first - coordinates[to].first;
    double dy = coordinates[from].second - coordinates[to].second;
    return std::sqrt(dx * dx + dy * dy);
  }

  CostT cost(VertexT from, VertexT to) const override
  {
    if (from >= vertexCount || to >= vertexCount)
      return infty;
    if (from == to)
      return 0.0;

    for (const auto &edge : adjacencyList[from])
    {
      if (edge.first == to)
      {
        return edge.second;
      }
    }
    return infty;
  }

  // Vis: Methoden für die SFML-Visualisierung
  const std::vector<std::pair<double, double>> &getCoordinates() const
  {
    return coordinates;
  }

  std::vector<std::tuple<VertexT, VertexT, CostT>> getAllEdges() const
  {
    std::vector<std::tuple<VertexT, VertexT, CostT>> edges;
    for (VertexT from = 0; from < vertexCount; ++from)
    {
      for (const auto &edge : adjacencyList[from])
      {
        edges.emplace_back(from, edge.first, edge.second);
      }
    }
    return edges;
  }

  // Eingabe-Operator für das Einlesen von koordinatenbasierten Graphdateien (Graph1-4)
  friend std::istream &operator>>(std::istream &is, CoordinateGraph &graph)
  {
    std::size_t numVertices, numEdges;
    is >> numVertices >> numEdges;

    graph.vertexCount = numVertices;
    graph.adjacencyList.resize(numVertices);
    graph.coordinates.resize(numVertices);

    // Lese Kanten: from, to, cost
    for (std::size_t i = 0; i < numEdges; ++i)
    {
      VertexT from, to;
      CostT cost;
      is >> from >> to >> cost;
      graph.adjacencyList[from].push_back({to, cost});
    }

    // Lese Koordinaten: lon, lat für jeden Knoten
    for (std::size_t i = 0; i < numVertices; ++i)
    {
      double lon, lat;
      is >> lon >> lat;
      graph.coordinates[i] = {lon, lat};
    }

    return is;
  }
};

// Graph für maze-basierte Probleme (Maze1-5 und zufällige Labyrinthe)
class MazeGraph : public DistanceGraph
{
private:
  std::size_t width, height;
  std::vector<std::vector<bool>> passable; // true if cell is passable
  VertexT startVertex = undefinedVertex, goalVertex = undefinedVertex;

  // Konvertiere 2D-Koordinaten in die Vertex-ID (row-major order)
  VertexT coordToVertex(std::size_t row, std::size_t col) const
  {
    return row * width + col;
  }

  // Konvertiere die Vertex-ID in 2D-Koordinaten
  std::pair<std::size_t, std::size_t> vertexToCoord(VertexT v) const
  {
    return {v / width, v % width};
  }

public:
  MazeGraph() = default;

  const NeighborT getNeighbors(VertexT v) const override
  {
    NeighborT neighbors;
    auto [row, col] = vertexToCoord(v);

    // Überprüfe alle 4 Richtungen (oben, unten, links, rechts)
    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i)
    {
      int newRow = static_cast<int>(row) + dr[i];
      int newCol = static_cast<int>(col) + dc[i];

      // Überprüfe Grenzen und Durchlässigkeit
      if (newRow >= 0 && newRow < static_cast<int>(height) &&
          newCol >= 0 && newCol < static_cast<int>(width))
      {
        if (passable[newRow][newCol])
        {
          VertexT neighbor = coordToVertex(newRow, newCol);
          neighbors.push_back({neighbor, 1.0}); // Alle Bewegungen kosten 1
        }
      }
    }
    return neighbors;
  }

  CostT estimatedCost(VertexT from, VertexT to) const override
  {
    auto [fromRow, fromCol] = vertexToCoord(from);
    auto [toRow, toCol] = vertexToCoord(to);

    // Manhattan-Distanz-Heuristik (zulässig für Gitter mit Einheitskosten)
    return std::abs(static_cast<int>(fromRow) - static_cast<int>(toRow)) +
           std::abs(static_cast<int>(fromCol) - static_cast<int>(toCol));
  }

  CostT cost(VertexT from, VertexT to) const override
  {
    if (from == to)
      return 0.0;

    auto neighbors = getNeighbors(from);
    for (const auto &neighbor : neighbors)
    {
      if (neighbor.first == to)
      {
        return neighbor.second;
      }
    }
    return infty;
  }

  // Vis: Methoden für die SFML-Visualisierung
  const std::vector<std::vector<bool>> &getWalls() const
  {
    return passable; // Note: this returns passable areas, not walls
  }

  std::size_t getWidth() const { return width; }
  std::size_t getHeight() const { return height; }

  // Zusätzliche Methoden (nicht in der Basisklassen-Schnittstelle)
  VertexT getStartVertex() const { return startVertex; }
  VertexT getGoalVertex() const { return goalVertex; }

  // Eingabe-Operator für das Einlesen von Labyrinth-Dateien (Maze1-5)
  friend std::istream &operator>>(std::istream &is, MazeGraph &graph)
  {
    is >> graph.height >> graph.width;
    graph.vertexCount = graph.width * graph.height;
    graph.passable.assign(graph.height, std::vector<bool>(graph.width, false));

    for (std::size_t row = 0; row < graph.height; ++row)
    {
      std::string line;
      is >> line;
      for (std::size_t col = 0; col < graph.width && col < line.size(); ++col)
      {
        if (line[col] == '.')
        {
          graph.passable[row][col] = true;
        }
        else if (line[col] == '#')
        {
          graph.passable[row][col] = false;
        }
        else if (line[col] == 'S' || line[col] == 's')
        {
          graph.passable[row][col] = true;
          graph.startVertex = graph.coordToVertex(row, col);
        }
        else if (line[col] == 'G' || line[col] == 'g' || line[col] == 'E' || line[col] == 'e')
        {
          graph.passable[row][col] = true;
          graph.goalVertex = graph.coordToVertex(row, col);
        }
      }
    }

    // Vis: Automatische Zuweisung von Start/Ziel, falls nicht gefunden
    if (graph.startVertex == undefinedVertex || graph.goalVertex == undefinedVertex)
    {
      // Erst passierbare Zelle für Start finden
      for (std::size_t row = 0; row < graph.height && graph.startVertex == undefinedVertex; ++row)
      {
        for (std::size_t col = 0; col < graph.width && graph.startVertex == undefinedVertex; ++col)
        {
          if (graph.passable[row][col])
          {
            graph.startVertex = graph.coordToVertex(row, col);
          }
        }
      }

      // Finde letzte passierbare Zelle für Ziel
      for (int row = graph.height - 1; row >= 0 && graph.goalVertex == undefinedVertex; --row)
      {
        for (int col = graph.width - 1; col >= 0 && graph.goalVertex == undefinedVertex; --col)
        {
          if (graph.passable[row][col])
          {
            graph.goalVertex = graph.coordToVertex(row, col);
          }
        }
      }
    }

    return is;
  }

  // Konstruktor für zufälliges Labyrinth (Beispiel 10)
  MazeGraph(const std::vector<CellType> &maze, std::size_t w, std::size_t h)
      : width(w), height(h)
  {
    vertexCount = width * height;
    passable.assign(height, std::vector<bool>(width, false));

    for (std::size_t i = 0; i < maze.size(); ++i)
    {
      std::size_t row = i / width;
      std::size_t col = i % width;

      switch (maze[i])
      {
      case CellType::Ground:
        passable[row][col] = true;
        break;
      case CellType::Start:
        passable[row][col] = true;
        startVertex = coordToVertex(row, col);
        break;
      case CellType::Destination:
        passable[row][col] = true;
        goalVertex = coordToVertex(row, col);
        break;
      case CellType::Wall:
        passable[row][col] = false;
        break;
      }
    }
  }
};

void Dijkstra(const DistanceGraph &g, GraphVisualizer &v, VertexT start,
              std::vector<CostT> &kostenZumStart)
{
  // Initialisiere Distanzen und besuchte Menge
  std::size_t n = g.numVertices();
  kostenZumStart.assign(n, infty);
  std::vector<bool> visited(n, false);

  kostenZumStart[start] = 0.0;

  // Priority queue: (cost, vertex) - min-heap
  std::priority_queue<std::pair<CostT, VertexT>,
                      std::vector<std::pair<CostT, VertexT>>,
                      std::greater<std::pair<CostT, VertexT>>>
      pq;

  pq.push({0.0, start});
  v.markVertex(start, VertexStatus::InQueue);

  while (!pq.empty())
  {
    auto [currentCost, current] = pq.top();
    pq.pop();

    // Überspringen, wenn bereits verarbeitet (kann bei doppelten Einträgen passieren)
    if (visited[current])
      continue;

    visited[current] = true;
    v.markVertex(current, VertexStatus::Active);

    // Verarbeite alle Nachbarn
    auto neighbors = g.getNeighbors(current);
    for (const auto &[neighbor, edgeCost] : neighbors)
    {
      CostT newCost = currentCost + edgeCost;

      // Aktualisiere die Distanz, wenn wir einen kürzeren Pfad gefunden haben
      if (newCost < kostenZumStart[neighbor])
      {
        kostenZumStart[neighbor] = newCost;
        pq.push({newCost, neighbor});
        v.markEdge({current, neighbor}, EdgeStatus::Active);
        v.updateVertex(neighbor, newCost, 0.0, current, VertexStatus::InQueue);
      }
    }

    v.markVertex(current, VertexStatus::Done);
    v.draw();
  }
}

// Vis: Funktion zum Offenhalten des SFML-Fensters nach Abschluss des Algorithmus
#ifdef SFML_ENABLED
void keepSFMLWindowOpen(GraphVisualizer &v)
{
  if (auto *sfmlViz = dynamic_cast<SFMLVisualizer *>(&v))
  {
    std::cout << "\n=== Algorithmus abgeschlossen! ===" << std::endl;
    std::cout << "Das Fenster zeigt das endgültige Ergebnis mit dem hervorgehobenen optimalen Pfad." << std::endl;
    std::cout << "Steuerung: SPACE=Pause, L=Labels umschalten, +/-=Geschwindigkeit, ESC=Beenden" << std::endl;

    while (sfmlViz->isOpen())
    {
      sf::sleep(sf::milliseconds(100));
      sfmlViz->draw();
    }
  }
}
#endif

// A* algorithm
bool A_star(const DistanceGraph &g, GraphVisualizer &v, VertexT start,
            VertexT ziel, std::list<VertexT> &weg)
{
  // Frühzeitige Rückgabe, wenn Start gleich Ziel
  if (start == ziel)
  {
    weg.push_back(start);
    return true;
  }

  std::size_t n = g.numVertices();
  std::vector<CostT> gScore(n, infty); // Kosten von Start zu Vertex
  std::vector<CostT> fScore(n, infty); // gScore + Heuristik
  std::vector<VertexT> parent(n, undefinedVertex);
  std::unordered_set<VertexT> closedSet;

  gScore[start] = 0.0;
  fScore[start] = g.estimatedCost(start, ziel);

  // Priority queue: (fScore, vertex) - min-heap
  std::priority_queue<std::pair<CostT, VertexT>,
                      std::vector<std::pair<CostT, VertexT>>,
                      std::greater<std::pair<CostT, VertexT>>>
      openSet;

  openSet.push({fScore[start], start});
  v.markVertex(start, VertexStatus::InQueue);

  while (!openSet.empty())
  {
    auto [currentF, current] = openSet.top();
    openSet.pop();

    // Überspringen, wenn bereits verarbeitet
    if (closedSet.count(current))
      continue;

// Vis: Aktiviere aktuellen Knoten für SFML-Visualisierung
#ifdef SFML_ENABLED
    if (auto *sfmlViz = dynamic_cast<SFMLVisualizer *>(&v))
    {
      sfmlViz->setCurrentActiveVertex(current);
      sfmlViz->clearCurrentNeighbors();
    }
#endif

    // Als aktiv markieren
    v.markVertex(current, VertexStatus::Active);

    // Überprüfen, ob wir das Ziel erreicht haben
    if (current == ziel)
    {
      v.markVertex(current, VertexStatus::Destination);

      // Rekonstruiere den Pfad mit Hilfe der Parent-Liste
      weg.clear();
      VertexT node = ziel;
      while (node != undefinedVertex)
      {
        weg.push_front(node);
        node = parent[node];
      }

// Vis: Aktiviere den endgültigen Pfad und markiere den Abschluss des Algorithmus für SFML
#ifdef SFML_ENABLED
      if (auto *sfmlViz = dynamic_cast<SFMLVisualizer *>(&v))
      {
        sfmlViz->setFinalPath(weg);
        sfmlViz->markOptimalPathEdges(weg);
        sfmlViz->markAlgorithmFinished();
        v.draw();              // Draw final state
        keepSFMLWindowOpen(v); // Keep window open to show result
      }
#endif

      return true;
    }

    closedSet.insert(current);
    v.markVertex(current, VertexStatus::Done);

    // Verarbeite Nachbarn
    auto neighbors = g.getNeighbors(current);
    for (const auto &[neighbor, edgeCost] : neighbors)
    {
      if (closedSet.count(neighbor))
        continue;

      CostT tentativeG = gScore[current] + edgeCost;

      // Wenn wir einen besseren Pfad zum Nachbarn gefunden haben
      if (tentativeG < gScore[neighbor])
      {
        parent[neighbor] = current;
        gScore[neighbor] = tentativeG;
        fScore[neighbor] = tentativeG + g.estimatedCost(neighbor, ziel);

        openSet.push({fScore[neighbor], neighbor});
        v.markEdge({current, neighbor}, EdgeStatus::Active);

// Vis: Aktiviere Nachbarn und setze Kantengewicht für SFML-Visualisierung
#ifdef SFML_ENABLED
        if (auto *sfmlViz = dynamic_cast<SFMLVisualizer *>(&v))
        {
          sfmlViz->addCurrentNeighbor(neighbor);
          sfmlViz->setEdgeWeight(current, neighbor, edgeCost);
        }
#endif

        v.updateVertex(neighbor, gScore[neighbor],
                       g.estimatedCost(neighbor, ziel), current,
                       VertexStatus::InQueue);
      }
    }
    v.draw();
  }

// Vis: Algorithmus abgeschlossen, auch wenn kein Pfad gefunden wurde
#ifdef SFML_ENABLED
  if (auto *sfmlViz = dynamic_cast<SFMLVisualizer *>(&v))
  {
    sfmlViz->markAlgorithmFinished();
    keepSFMLWindowOpen(v);
  }
#endif

  return false; // Kein Pfad gefunden.
}

int main()
{
  // Frage Beispielnummer vom User ab
  std::cout << "=== A* Algorithm Visualization ===" << std::endl;
  std::cout << "Geben Sie die Beispielnummer ein (1-10): ";
  int beispiel;
  std::cin >> beispiel;

  // Vis: Visualisierungsauswahl
  int visualChoice = 1; // Default to text
#ifdef SFML_ENABLED
  std::cout << "\nWelche Visualisierung möchten Sie verwenden?" << std::endl;
  std::cout << "1. Text-Visualisierung" << std::endl;
  std::cout << "2. SFML-Visualisierung" << std::endl;
  std::cout << "Ihre Wahl (1 oder 2): ";
  std::cin >> visualChoice;
#endif

  if (beispiel >= 1 && beispiel <= 4)
  {
    // Route graphs (Graph1-4.dat)
    std::string filename = "daten/Graph" + std::to_string(beispiel) + ".dat";
    std::ifstream file(filename);

    if (!file.is_open())
    {
      std::cerr << "Fehler: Datei " << filename << " konnte nicht geöffnet werden." << std::endl;
      return 1;
    }

    CoordinateGraph graph;
    file >> graph;
    file.close();

    // PruefeHeuristik
    if (!PruefeHeuristik(graph))
    {
      std::cerr << "Heuristik ist nicht zulässig!" << std::endl;
      return 1;
    }

    std::cout << "Graph geladen. Anzahl Knoten: " << graph.numVertices() << std::endl;
    std::cout << "Heuristik ist zulässig." << std::endl;

    // Vis: Erstelle Visualizer basierend auf der Wahl
    std::unique_ptr<GraphVisualizer> visualizer;
    if (visualChoice == 2)
    {
#ifdef SFML_ENABLED
      // Fenstergrößen wie im PDF empfohlen
      int width = (beispiel == 3) ? 1300 : ((beispiel == 4) ? 700 : 1000);
      int height = (beispiel == 3) ? 800 : ((beispiel == 4) ? 800 : 1000);

      auto routeViz = std::make_unique<RouteGraphVisualizer>(
          width, height, graph.getCoordinates());

      // Alle Kanten für die Visualisierung vorladen
      auto edges = graph.getAllEdges();
      std::cout << "Loading " << edges.size() << " edges for visualization..." << std::endl;
      for (const auto &[from, to, weight] : edges)
      {
        routeViz->setEdgeWeight(from, to, weight);
        routeViz->markEdge({from, to}, EdgeStatus::UnknownEdge);
      }

      visualizer = std::move(routeViz);
      std::cout << "SFML-Visualisierung aktiviert!" << std::endl;
      std::cout << "Controls: SPACE=Pause, S=Step, L=Labels, +/-=Speed, H=Help, ESC=Exit" << std::endl;
#endif
    }
    else
    {
      visualizer = std::make_unique<TextVisualizer>();
    }

    if (visualChoice == 2)
    {
      // SFML: Zeige eine repräsentative A*-Visualisierung
      VertexT goal = std::min(static_cast<VertexT>(graph.numVertices() - 2), static_cast<VertexT>(5));
      std::cout << "\nZeige A*-Visualisierung von Knoten 1 zu Knoten " << goal << std::endl;
      std::list<VertexT> path;
      if (A_star(graph, *visualizer, 1, goal, path))
      {
        PruefeWeg(beispiel, path);
        std::cout << "Visualisierung abgeschlossen." << std::endl;
      }
    }
    else
    {
      // Text mode: Run ALL comprehensive tests (as in your original code)
      std::cout << "Teste Dijkstra für alle Startknoten..." << std::endl;
      for (VertexT start = 0; start < graph.numVertices(); ++start)
      {
        std::vector<CostT> distances;
        Dijkstra(graph, *visualizer, start, distances);
        PruefeDijkstra(beispiel, start, distances);
      }
      std::cout << "Dijkstra-Tests erfolgreich!" << std::endl;

      std::cout << "Drücken Sie die Eingabetaste, um fortzufahren..." << std::endl;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear leftover input
      std::cin.get();

      // Testen A* für alle Knotenpaare
      std::cout << "Teste A* für alle Knotenpaare..." << std::endl;
      for (VertexT start = 0; start < graph.numVertices(); ++start)
      {
        for (VertexT goal = 0; goal < graph.numVertices(); ++goal)
        {
          if (start != goal)
          {
            std::list<VertexT> path;
            if (A_star(graph, *visualizer, start, goal, path))
            {
              PruefeWeg(beispiel, path);
            }
            else
            {
              std::cout << "Kein Weg von " << start << " zu " << goal << " gefunden." << std::endl;
            }
          }
        }
      }
      std::cout << "A*-Tests erfolgreich!" << std::endl;
    }
  }
  else if (beispiel >= 5 && beispiel <= 9)
  {
    // Maze graphs (Maze1-5.dat)
    std::string filename = "daten/Maze" + std::to_string(beispiel - 4) + ".dat";
    std::ifstream file(filename);

    if (!file.is_open())
    {
      std::cerr << "Fehler: Datei " << filename << " konnte nicht geöffnet werden." << std::endl;
      return 1;
    }

    MazeGraph graph;
    file >> graph;
    file.close();

    // PruefeHeuristik
    if (!PruefeHeuristik(graph))
    {
      std::cerr << "Heuristik ist nicht zulässig!" << std::endl;
      return 1;
    }

    std::cout << "Labyrinth geladen. Anzahl Knoten: " << graph.numVertices() << std::endl;
    std::cout << "Heuristik ist zulässig." << std::endl;

    // Vis: Erstelle Visualizer basierend auf der Wahl
    std::unique_ptr<GraphVisualizer> visualizer;
    if (visualChoice == 2)
    {
#ifdef SFML_ENABLED
      visualizer = std::make_unique<MazeVisualizer>(
          800, 800,
          graph.getWidth(), graph.getHeight(),
          graph.getWalls(),
          graph.getStartVertex(),
          graph.getGoalVertex());
      std::cout << "SFML-Visualisierung aktiviert!" << std::endl;
      std::cout << "Controls: SPACE=Pause, S=Step, L=Labels, +/-=Speed, H=Help, ESC=Exit" << std::endl;
#endif
    }
    else
    {
      visualizer = std::make_unique<TextVisualizer>();
    }

    // Löse die in der Aufgabenstellung beschriebenen Probleme für maze graphs
    // Bestimme Start-Ziel-Paare für dieses Beispiel
    auto pairs = StartZielPaare(beispiel);
    std::cout << "Teste A* für " << pairs.size() << " Start-Ziel-Paare..." << std::endl;

    for (const auto &[start, goal] : pairs)
    {
      std::list<VertexT> path;
      if (A_star(graph, *visualizer, start, goal, path))
      {
        PruefeWeg(beispiel, path);
        std::cout << "Weg von " << start << " zu " << goal << " gefunden (Länge: " << path.size() - 1 << ")" << std::endl;
      }
      else
      {
        std::cout << "Kein Weg von " << start << " zu " << goal << " gefunden." << std::endl;
      }
    }
    std::cout << "Maze A*-Tests erfolgreich!" << std::endl;
  }
  else if (beispiel == 10)
  {
    // Random maze
    const int width = 256, height = 256;
    const unsigned int seed = 42; // Fiksierter Seed für Reproduzierbarkeit

    auto mazeData = ErzeugeLabyrinth(width, height, seed);
    MazeGraph graph(mazeData, width, height);

    std::cout << "Prüfe Heuristik für zufälliges Labyrinth..." << std::endl;
    // PruefeHeuristik
    if (!PruefeHeuristik(graph))
    {
      std::cerr << "Heuristik ist nicht zulässig!" << std::endl;
      return 1;
    }

    std::cout << "Zufälliges Labyrinth erzeugt. Größe: " << width << "x" << height << std::endl;
    std::cout << "Heuristik ist zulässig." << std::endl;

    // ENHANCED: Create visualizer based on choice
    std::unique_ptr<GraphVisualizer> visualizer;
    if (visualChoice == 2)
    {
#ifdef SFML_ENABLED
      // Verwende kleinere Anzeige für bessere Visualisierung großer Labyrinthe
      visualizer = std::make_unique<MazeVisualizer>(
          800, 800,
          graph.getWidth(), graph.getHeight(),
          graph.getWalls(),
          graph.getStartVertex(),
          graph.getGoalVertex());
      std::cout << "SFML-Visualisierung aktiviert!" << std::endl;
      std::cout << "Large maze - use L to toggle labels for better performance!" << std::endl;
      std::cout << "Controls: SPACE=Pause, S=Step, L=Labels, +/-=Speed, H=Help, ESC=Exit" << std::endl;
#endif
    }
    else
    {
      visualizer = std::make_unique<TextVisualizer>();
    }

    // Löse die in der Aufgabenstellung beschriebenen Probleme für random maze
    // Finde den Pfad vom Start- zum Zielknoten
    VertexT start = graph.getStartVertex();
    VertexT goal = graph.getGoalVertex();

    std::cout << "Teste A* im zufälligen Labyrinth von " << start << " zu " << goal << "..." << std::endl;
    std::list<VertexT> path;
    if (A_star(graph, *visualizer, start, goal, path))
    {
      PruefeWeg(beispiel, path);
      std::cout << "Weg gefunden! Länge: " << path.size() - 1 << std::endl;
    }
    else
    {
      std::cout << "Kein Weg gefunden." << std::endl;
    }
    std::cout << "Random Maze A*-Test erfolgreich!" << std::endl;
  }
  else
  {
    std::cerr << "Ungültige Beispielnummer! Bitte wählen Sie 1-10." << std::endl;
    return 1;
  }

  std::cout << "Alle Tests erfolgreich abgeschlossen!" << std::endl;
  return 0;
}