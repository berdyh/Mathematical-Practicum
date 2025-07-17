
#include "astern/text_visualizer.h"
#include "astern/unit.h"
#include <iostream>
#include <vector>
#include <cmath>

// Ein Graph, der Koordinaten von Knoten speichert.
class CoordinateGraph : public DistanceGraph {
private:
  std::vector<std::pair<double, double>> coordinates; // (lat, lon) for each vertex
  std::vector<std::vector<std::pair<VertexT, CostT>>> adjacencyList;

public:
  CoordinateGraph() = default;

  const NeighborT getNeighbors(VertexT v) const override {
    if (v >= vertexCount) return NeighborT();
    
    NeighborT neighbors;
    for (const auto& edge : adjacencyList[v]) {
      neighbors.push_back({edge.first, edge.second});
    }
    return neighbors;
  }

  CostT estimatedCost(VertexT from, VertexT to) const override {
    if (from >= coordinates.size() || to >= coordinates.size()) {
      return infty;
    }
    
    // Use Euclidean distance as heuristic
    double dx = coordinates[from].first - coordinates[to].first;
    double dy = coordinates[from].second - coordinates[to].second;
    return std::sqrt(dx * dx + dy * dy);
  }

  CostT cost(VertexT from, VertexT to) const override {
    if (from >= vertexCount || to >= vertexCount) return infty;
    if (from == to) return 0.0;
    
    for (const auto& edge : adjacencyList[from]) {
      if (edge.first == to) {
        return edge.second;
      }
    }
    return infty;
  }
    
  // Input operator for reading coordinate-based graph files (Graph1-4)
  friend std::istream& operator>>(std::istream& is, CoordinateGraph& graph) {
    std::size_t numVertices, numEdges;
    is >> numVertices >> numEdges;
    
    graph.vertexCount = numVertices;
    graph.adjacencyList.resize(numVertices);
    graph.coordinates.resize(numVertices);
    
    // Read edges: from, to, cost
    for (std::size_t i = 0; i < numEdges; ++i) {
      VertexT from, to;
      CostT cost;
      is >> from >> to >> cost;
      graph.adjacencyList[from].push_back({to, cost});
    }
    
    // Read coordinates: lat, lon for each vertex
    for (std::size_t i = 0; i < numVertices; ++i) {
      double lat, lon;
      is >> lat >> lon;
      graph.coordinates[i] = {lat, lon};
    }
    
    return is;
  }
};
void Dijkstra(const DistanceGraph& g, GraphVisualizer& v, VertexT start,
              std::vector<CostT>& kostenZumStart) {
  // ...
}

bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start,
            VertexT ziel, std::list<VertexT>& weg) {
  // ...
  return false;  // Kein Weg gefunden.
}

int main() {
  // Frage Beispielnummer vom User ab

  // Lade die zugehoerige Textdatei in einen Graphen
  // PruefeHeuristik

  // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige
  // Datei PruefeDijkstra / PruefeWeg

  return 0;
}
