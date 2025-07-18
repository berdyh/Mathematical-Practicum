#ifndef SFML_VISUALIZER_H_
#define SFML_VISUALIZER_H_

#include "astern/unit.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>

// Structure to hold vertex visualization data
struct VertexData {
    VertexStatus status = VertexStatus::UnknownVertex;
    double gCost = 0.0;
    double hCost = 0.0;
    VertexT parent = undefinedVertex;
    sf::Vector2f position;
    bool showLabels = true;
};

// Structure to hold edge visualization data
struct EdgeData {
    EdgeStatus status = EdgeStatus::UnknownEdge;
    CostT weight = 0.0;
    VertexT from;
    VertexT to;
};

// Base class for SFML-based visualizers
class SFMLVisualizer : public GraphVisualizer {
protected:
    sf::RenderWindow window;
    sf::Font font;
    std::unordered_map<VertexT, VertexData> vertexData;
    std::unordered_map<std::size_t, EdgeData> edgeData;
    
    // Visualization control
    bool isPaused = false;
    bool stepMode = false;
    int animationSpeed = 50; // milliseconds delay
    bool algorithmFinished = false;
    std::list<VertexT> finalPath;
    
    // Colors for different states
    sf::Color getVertexColor(VertexStatus status);
    sf::Color getEdgeColor(EdgeStatus status);
    
    // Helper function to create edge key
    std::size_t getEdgeKey(VertexT from, VertexT to);
    
    // Helper function to draw text
    void drawText(const std::string& text, sf::Vector2f position, int size = 12);
    
    // Event handling
    virtual void handleEvents();
    
    // Draw help text
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
    
    // Additional methods
    bool isOpen() const { return window.isOpen(); }
    void toggleLabels();
    void setFinalPath(const std::list<VertexT>& path);
    void markAlgorithmFinished();
    
    // Method to set edge weight (needed for visualization)
    void setEdgeWeight(VertexT from, VertexT to, CostT weight);
};

// Maze-specific visualizer
class MazeVisualizer : public SFMLVisualizer {
private:
    std::size_t mazeWidth, mazeHeight;
    float cellSize;
    std::vector<std::vector<bool>> walls;
    
    // Convert vertex ID to maze coordinates
    sf::Vector2i vertexToMazeCoord(VertexT vertex);
    
    // Convert maze coordinates to screen position
    sf::Vector2f mazeCoordToScreen(int row, int col);
    
    // Draw the maze content (separate from window management)
    void drawMazeContent();

public:
    MazeVisualizer(int windowWidth, int windowHeight, 
                   std::size_t mazeW, std::size_t mazeH,
                   const std::vector<std::vector<bool>>& wallData);
    
    void draw() override;
};

// Route graph specific visualizer  
class RouteGraphVisualizer : public SFMLVisualizer {
private:
    std::vector<sf::Vector2f> nodePositions;
    sf::Vector2f minCoord, maxCoord;
    float scaleFactor;
    
    // Convert graph coordinates to screen coordinates
    sf::Vector2f graphCoordToScreen(double x, double y);
    
    // Draw edges
    void drawEdges();
    void drawNodes();

public:
    RouteGraphVisualizer(int windowWidth, int windowHeight,
                        const std::vector<std::pair<double, double>>& coordinates);
    
    void draw() override;
};

#endif // SFML_VISUALIZER_H_