#include "astern/sfml_visualizer.h"
#include <iostream>
#include <sstream>
#include <iomanip>

SFMLVisualizer::SFMLVisualizer(int width, int height, const std::string& title) 
    : window(sf::VideoMode(width, height), title) {
    
    // Load font
    if (!font.loadFromFile("data/font/BebasNeue-Regular.ttf")) {
        std::cerr << "Warning: Could not load font. Text will not be displayed." << std::endl;
    }
    
    // Set window properties
    window.setFramerateLimit(60);
}

sf::Color SFMLVisualizer::getVertexColor(VertexStatus status) {
    switch (status) {
        case VertexStatus::UnknownVertex:
            return sf::Color(128, 128, 128);  // Gray
        case VertexStatus::InQueue:
            return sf::Color(255, 255, 0);    // Yellow
        case VertexStatus::Active:
            return sf::Color(255, 0, 0);      // Red
        case VertexStatus::Done:
            return sf::Color(0, 255, 0);      // Green
        case VertexStatus::Destination:
            return sf::Color(0, 0, 255);      // Blue
        default:
            return sf::Color::White;
    }
}

sf::Color SFMLVisualizer::getEdgeColor(EdgeStatus status) {
    switch (status) {
        case EdgeStatus::UnknownEdge:
            return sf::Color(100, 100, 100);  // Dark gray
        case EdgeStatus::Visited:
            return sf::Color(200, 200, 200);  // Light gray
        case EdgeStatus::Active:
            return sf::Color(255, 0, 0);      // Red
        case EdgeStatus::Optimal:
            return sf::Color(0, 255, 0);      // Green
        default:
            return sf::Color::White;
    }
}

std::size_t SFMLVisualizer::getEdgeKey(VertexT from, VertexT to) {
    // Create unique key for edge (order matters for directed graphs)
    return static_cast<std::size_t>(from) * 1000000 + static_cast<std::size_t>(to);
}

void SFMLVisualizer::drawText(const std::string& text, sf::Vector2f position, int size) {
    if (!font.getInfo().family.empty()) {
        sf::Text textObj;
        textObj.setFont(font);
        textObj.setString(text);
        textObj.setCharacterSize(size);
        textObj.setFillColor(sf::Color::Black);
        textObj.setPosition(position);
        window.draw(textObj);
    }
}

void SFMLVisualizer::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::L) {
                toggleLabels();
            }
            else if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
    }
}

void SFMLVisualizer::markVertex(VertexT vertex, VertexStatus status) {
    vertexData[vertex].status = status;
}

void SFMLVisualizer::markEdge(EdgeT e, EdgeStatus status) {
    std::size_t key = getEdgeKey(e.first, e.second);
    edgeData[key].status = status;
    edgeData[key].from = e.first;
    edgeData[key].to = e.second;
}

void SFMLVisualizer::updateVertex(VertexT vertex, double cost, double estimate,
                                 VertexT parent, VertexStatus status) {
    auto& data = vertexData[vertex];
    data.status = status;
    data.gCost = cost;
    data.hCost = estimate;
    data.parent = parent;
}

void SFMLVisualizer::draw() {
    handleEvents();
    window.clear(sf::Color::White);
    
    // Derived classes will implement specific drawing
    
    window.display();
    
    // Add small delay to make visualization visible
    sf::sleep(sf::milliseconds(50));
}

void SFMLVisualizer::toggleLabels() {
    for (auto& pair : vertexData) {
        pair.second.showLabels = !pair.second.showLabels;
    }
}

void SFMLVisualizer::setEdgeWeight(VertexT from, VertexT to, CostT weight) {
    std::size_t key = getEdgeKey(from, to);
    edgeData[key].weight = weight;
    edgeData[key].from = from;
    edgeData[key].to = to;
}

// MazeVisualizer implementation
MazeVisualizer::MazeVisualizer(int windowWidth, int windowHeight, 
                              std::size_t mazeW, std::size_t mazeH,
                              const std::vector<std::vector<bool>>& wallData)
    : SFMLVisualizer(windowWidth, windowHeight, "Maze A* Visualization"),
      mazeWidth(mazeW), mazeHeight(mazeH), walls(wallData) {
    
    // Calculate cell size based on window size and maze dimensions
    cellSize = std::min(static_cast<float>(windowWidth) / mazeWidth,
                       static_cast<float>(windowHeight) / mazeHeight);
    
    // Initialize vertex positions
    for (std::size_t row = 0; row < mazeHeight; ++row) {
        for (std::size_t col = 0; col < mazeWidth; ++col) {
            VertexT vertex = row * mazeWidth + col;
            vertexData[vertex].position = mazeCoordToScreen(row, col);
        }
    }
}

sf::Vector2i MazeVisualizer::vertexToMazeCoord(VertexT vertex) {
    return sf::Vector2i(vertex % mazeWidth, vertex / mazeWidth);
}

sf::Vector2f MazeVisualizer::mazeCoordToScreen(int row, int col) {
    return sf::Vector2f(col * cellSize + cellSize/2, row * cellSize + cellSize/2);
}

void MazeVisualizer::draw() {
    handleEvents();
    window.clear(sf::Color::White);
    
    // Draw maze structure
    for (std::size_t row = 0; row < mazeHeight; ++row) {
        for (std::size_t col = 0; col < mazeWidth; ++col) {
            sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
            cell.setPosition(col * cellSize, row * cellSize);
            
            if (walls[row][col]) {
                // Wall
                cell.setFillColor(sf::Color::Black);
            } else {
                // Passable area
                VertexT vertex = row * mazeWidth + col;
                auto it = vertexData.find(vertex);
                if (it != vertexData.end()) {
                    cell.setFillColor(getVertexColor(it->second.status));
                } else {
                    cell.setFillColor(sf::Color::White);
                }
            }
            
            window.draw(cell);
            
            // Draw labels if enabled and cell is passable
            if (!walls[row][col]) {
                VertexT vertex = row * mazeWidth + col;
                auto it = vertexData.find(vertex);
                if (it != vertexData.end() && it->second.showLabels && 
                    it->second.status != VertexStatus::UnknownVertex) {
                    
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(1) 
                       << it->second.gCost << "+" << it->second.hCost;
                    
                    sf::Vector2f textPos = mazeCoordToScreen(row, col);
                    textPos.x -= 15; // Center text
                    textPos.y -= 8;
                    drawText(ss.str(), textPos, 8);
                }
            }
        }
    }
    
    window.display();
    sf::sleep(sf::milliseconds(50));
}

// RouteGraphVisualizer implementation
RouteGraphVisualizer::RouteGraphVisualizer(int windowWidth, int windowHeight,
                                          const std::vector<std::pair<double, double>>& coordinates)
    : SFMLVisualizer(windowWidth, windowHeight, "Route Graph A* Visualization"),
      nodePositions(coordinates.size()) {
    
    if (coordinates.empty()) {
        return;
    }
    
    // Find coordinate bounds
    minCoord = maxCoord = sf::Vector2f(coordinates[0].first, coordinates[0].second);
    
    for (const auto& coord : coordinates) {
        minCoord.x = std::min(minCoord.x, static_cast<float>(coord.first));
        minCoord.y = std::min(minCoord.y, static_cast<float>(coord.second));
        maxCoord.x = std::max(maxCoord.x, static_cast<float>(coord.first));
        maxCoord.y = std::max(maxCoord.y, static_cast<float>(coord.second));
    }
    
    // Calculate scale factor (leave some margin)
    float marginX = windowWidth * 0.1f;
    float marginY = windowHeight * 0.1f;
    float scaleX = (windowWidth - 2 * marginX) / (maxCoord.x - minCoord.x);
    float scaleY = (windowHeight - 2 * marginY) / (maxCoord.y - minCoord.y);
    scaleFactor = std::min(scaleX, scaleY);
    
    // Pre-calculate screen positions for all nodes
    for (std::size_t i = 0; i < coordinates.size(); ++i) {
        nodePositions[i] = graphCoordToScreen(coordinates[i].first, coordinates[i].second);
        vertexData[i].position = nodePositions[i];
    }
}

sf::Vector2f RouteGraphVisualizer::graphCoordToScreen(double x, double y) {
    // Convert graph coordinates to screen coordinates with margins
    float marginX = window.getSize().x * 0.1f;
    float marginY = window.getSize().y * 0.1f;
    
    float screenX = marginX + (x - minCoord.x) * scaleFactor;
    float screenY = marginY + (y - minCoord.y) * scaleFactor;
    
    return sf::Vector2f(screenX, screenY);
}

void RouteGraphVisualizer::drawEdges() {
    // Draw all edges
    for (const auto& edgePair : edgeData) {
        const EdgeData& edge = edgePair.second;
        
        if (edge.from < nodePositions.size() && edge.to < nodePositions.size()) {
            sf::Vector2f fromPos = nodePositions[edge.from];
            sf::Vector2f toPos = nodePositions[edge.to];
            
            // Create line
            sf::Vertex line[] = {
                sf::Vertex(fromPos, getEdgeColor(edge.status)),
                sf::Vertex(toPos, getEdgeColor(edge.status))
            };
            
            window.draw(line, 2, sf::Lines);
            
            // Draw edge weight if it's an active or optimal edge
            if (edge.status == EdgeStatus::Active || edge.status == EdgeStatus::Optimal) {
                sf::Vector2f midPoint = (fromPos + toPos) / 2.0f;
                std::stringstream ss;
                ss << std::fixed << std::setprecision(1) << edge.weight;
                drawText(ss.str(), midPoint, 10);
            }
        }
    }
}

void RouteGraphVisualizer::drawNodes() {
    // Draw all nodes
    for (const auto& vertexPair : vertexData) {
        VertexT vertex = vertexPair.first;
        const VertexData& data = vertexPair.second;
        
        if (vertex < nodePositions.size()) {
            sf::Vector2f pos = nodePositions[vertex];
            
            // Draw node circle
            sf::CircleShape node(8.0f);
            node.setFillColor(getVertexColor(data.status));
            node.setOutlineThickness(2.0f);
            node.setOutlineColor(sf::Color::Black);
            node.setOrigin(8.0f, 8.0f);
            node.setPosition(pos);
            
            window.draw(node);
            
            // Draw node labels if enabled
            if (data.showLabels && data.status != VertexStatus::UnknownVertex) {
                std::stringstream ss;
                ss << std::fixed << std::setprecision(1) 
                   << data.gCost << "+" << data.hCost;
                
                sf::Vector2f textPos = pos;
                textPos.x += 12; // Offset text to avoid overlap with node
                textPos.y -= 8;
                drawText(ss.str(), textPos, 10);
            }
            
            // Draw vertex ID
            std::stringstream idss;
            idss << vertex;
            sf::Vector2f idPos = pos;
            idPos.x -= 5;
            idPos.y += 12;
            drawText(idss.str(), idPos, 8);
        }
    }
}

void RouteGraphVisualizer::draw() {
    handleEvents();
    window.clear(sf::Color::White);
    
    // Draw edges first (so they appear behind nodes)
    drawEdges();
    
    // Draw nodes on top
    drawNodes();
    
    window.display();
    sf::sleep(sf::milliseconds(100)); // Slower for route graphs to see details
}
