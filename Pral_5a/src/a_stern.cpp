
#include "astern/text_visualizer.h"
#include "astern/unit.h"
#include <iostream>
#include <vector>
#include <cmath>

// Ein Graph, der Koordinaten von Knoten speichert.
class CoordinateGraph : public DistanceGraph
{
private:
  std::vector<std::pair<double, double>> coordinates; // (lat, lon) for each vertex
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

    // Lese Koordinaten: lat, lon für jeden Knoten
    for (std::size_t i = 0; i < numVertices; ++i)
    {
      double lat, lon;
      is >> lat >> lon;
      graph.coordinates[i] = {lat, lon};
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
  VertexT startVertex, goalVertex;

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
      for (std::size_t col = 0; col < graph.width; ++col)
      {
        if (line[col] == '.')
        {
          graph.passable[row][col] = true;
        }
        else if (line[col] == '#')
        {
          graph.passable[row][col] = false;
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
  // ...
}

bool A_star(const DistanceGraph &g, GraphVisualizer &v, VertexT start,
            VertexT ziel, std::list<VertexT> &weg)
{
  // ...
  return false; // Kein Weg gefunden.
}

int main()
{
  // Frage Beispielnummer vom User ab

  // Lade die zugehoerige Textdatei in einen Graphen
  // PruefeHeuristik

  // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige
  // Datei PruefeDijkstra / PruefeWeg

  return 0;
}
