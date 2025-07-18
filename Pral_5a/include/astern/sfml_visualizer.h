#ifndef SFML_VISUALIZER_H_
#define SFML_VISUALIZER_H_

#include "astern/unit.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>

// Structure to hold vertex visualization data
struct VertexData {
    VertexStatus status = VertexStatus::UnknownVertex;
    double gCost = 0.0;
    double hCost = 0.0;
    VertexT parent = undefinedVertex;
    sf::Vector2f position;
    bool showLabels = true;
    bool isCurrentlyActive = false;      // Currently being examined
    bool isNeighborInQueue = false;      // Neighbor being added to queue
};

// Structure to hold edge visualization data
struct EdgeData {
    EdgeStatus status = EdgeStatus::UnknownEdge;
    CostT weight = 0.0;
    VertexT from;
    VertexT to;
    bool isOnOptimalPath = false;
};

// Base class for SFML-based visualizers
class SFMLVisualizer : public GraphVisualizer {
protected:
    sf::RenderWindow window;
    sf::Font font;
    std::unordered_map<VertexT, VertexData> vertexData;
    std::unordered_map<std::size_t, EdgeData> edgeData;
    
    // Visualization control
    bool fastForward = true;
    bool stepMode = false;
    int animationSpeed = 750; // milliseconds delay
    bool algorithmFinished = false;
    std::list<VertexT> finalPath;
    bool showLabels = true;
    
    // Current algorithm state
    VertexT currentActiveVertex = undefinedVertex;
    std::unordered_set<VertexT> currentNeighbors;
    
    // Helper methods
    bool shouldProceed() const;
    
    // Colors for different states
    sf::Color getVertexColor(const VertexData& data);
    sf::Color getEdgeColor(EdgeStatus status);
    
    // Helper functions
    std::size_t getEdgeKey(VertexT from, VertexT to);
    void drawText(const std::string& text, sf::Vector2f position, int size = 12, sf::Color color = sf::Color::Black);
    virtual void handleEvents();
    void drawHelpText();

public:
    SFMLVisualizer(int width, int height, const std::string& title);
    virtual ~SFMLVisualizer() = default;
    
    // GraphVisualizer interface
    void markVertex(VertexT vertex, VertexStatus status) override;
    void markEdge(EdgeT e, EdgeStatus status) override;
    void updateVertex(VertexT vertex, double cost, double estimate,
                     VertexT parent, VertexStatus status) override;
    void draw() override;
    
    // Enhanced methods for better visualization
    void setCurrentActiveVertex(VertexT vertex);
    void clearCurrentNeighbors();
    void addCurrentNeighbor(VertexT vertex);
    void setFinalPath(const std::list<VertexT>& path);
    void markAlgorithmFinished();
    void setEdgeWeight(VertexT from, VertexT to, CostT weight);
    void markOptimalPathEdges(const std::list<VertexT>& path);
    
    // Control methods
    bool isOpen() const { return window.isOpen(); }
    void toggleLabels();
};

// Maze-specific visualizer
class MazeVisualizer : public SFMLVisualizer {
private:
    std::size_t mazeWidth, mazeHeight;
    float cellSize;
    std::vector<std::vector<bool>> passableAreas; // true = passable, false = wall
    VertexT startVertex, goalVertex;
    
    // Helper methods
    sf::Vector2i vertexToMazeCoord(VertexT vertex);
    sf::Vector2f mazeCoordToScreen(int row, int col);
    void drawMazeContent();

public:
    MazeVisualizer(int windowWidth, int windowHeight, 
                   std::size_t mazeW, std::size_t mazeH,
                   const std::vector<std::vector<bool>>& passable,
                   VertexT start, VertexT goal);
    
    void draw() override;
};

// Route graph specific visualizer  
class RouteGraphVisualizer : public SFMLVisualizer {
private:
    std::vector<sf::Vector2f> nodePositions;
    sf::Vector2f minCoord, maxCoord;
    float scaleFactor;
    
    // Helper methods
    sf::Vector2f graphCoordToScreen(double x, double y);
    void drawArrow(sf::Vector2f from, sf::Vector2f to, sf::Color color, float thickness = 2.0f);
    void drawEdges();
    void drawNodes();

public:
    RouteGraphVisualizer(int windowWidth, int windowHeight,
                        const std::vector<std::pair<double, double>>& coordinates);
    
    void draw() override;
};

#endif // SFML_VISUALIZER_H_