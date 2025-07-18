#include "astern/sfml_visualizer.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

// ============================================================================
// SFMLVisualizer Base Class Implementation
// ============================================================================

SFMLVisualizer::SFMLVisualizer(int width, int height, const std::string &title)
    : window(sf::VideoMode(width, height), title)
{

    // Try different font paths - this handles different working directories
    std::vector<std::string> fontPaths = {
        "data/font/BebasNeue-Regular.ttf",
        "font/BebasNeue-Regular.ttf",
        "../data/font/BebasNeue-Regular.ttf",
        "./data/font/BebasNeue-Regular.ttf"};

    bool fontLoaded = false;
    for (const auto &path : fontPaths)
    {
        if (font.loadFromFile(path))
        {
            std::cout << "Font loaded successfully from: " << path << std::endl;
            fontLoaded = true;
            break;
        }
    }

    if (!fontLoaded)
    {
        std::cerr << "Warning: Could not load font from any path. Text will not be displayed optimally." << std::endl;
    }

    window.setFramerateLimit(60);
}

sf::Color SFMLVisualizer::getVertexColor(const VertexData &data)
{
    // Priority order: current active > neighbors in queue > final status
    if (data.isCurrentlyActive)
    {
        return sf::Color(255, 0, 0); // Bright red for currently active vertex
    }
    if (data.isNeighborInQueue)
    {
        return sf::Color(255, 165, 0); // Orange for neighbors being processed
    }

    // Standard A* status colors (matching the PDF images)
    switch (data.status)
    {
    case VertexStatus::UnknownVertex:
        return sf::Color(240, 240, 240); // Light gray for unknown
    case VertexStatus::InQueue:
        return sf::Color(255, 255, 0); // Yellow for in queue
    case VertexStatus::Active:
        return sf::Color(255, 100, 100); // Light red for active
    case VertexStatus::Done:
        return sf::Color(144, 238, 144); // Light green for done
    case VertexStatus::Destination:
        return sf::Color(0, 0, 255); // Blue for destination
    default:
        return sf::Color::White;
    }
}

sf::Color SFMLVisualizer::getEdgeColor(EdgeStatus status)
{
    switch (status)
    {
    case EdgeStatus::UnknownEdge:
        return sf::Color(150, 150, 150); // Gray for unknown edges
    case EdgeStatus::Visited:
        return sf::Color(200, 200, 200); // Light gray for visited
    case EdgeStatus::Active:
        return sf::Color(255, 0, 0); // Red for currently active edge
    case EdgeStatus::Optimal:
        return sf::Color(0, 200, 0); // Green for optimal path
    default:
        return sf::Color(100, 100, 100);
    }
}

std::size_t SFMLVisualizer::getEdgeKey(VertexT from, VertexT to)
{
    // Create unique key for directed edge
    return static_cast<std::size_t>(from) * 1000000 + static_cast<std::size_t>(to);
}

void SFMLVisualizer::drawText(const std::string &text, sf::Vector2f position, int size, sf::Color color)
{
    if (!font.getInfo().family.empty())
    {
        sf::Text textObj;
        textObj.setFont(font);
        textObj.setString(text);
        textObj.setCharacterSize(size);
        textObj.setFillColor(color);
        textObj.setPosition(position);
        window.draw(textObj);
    }
}

void SFMLVisualizer::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                window.close();
                break;
            case sf::Keyboard::Space:
                fastForward = !fastForward;
                std::cout << (fastForward ? "Fast forward mode ON" : "Fast forward mode OFF") << std::endl;
                break;
            case sf::Keyboard::S:
                stepMode = !stepMode;
                if (stepMode)
                {
                    fastForward = false;      // Start in paused state when entering step mode
                    animationSpeed = 100; // Reset to normal speed
                    std::cout << "Step Mode ON - Press ENTER to advance steps" << std::endl;
                }
                else
                {
                    fastForward = true; // Resume normal operation
                    animationSpeed = 750;
                    std::cout << "Step Mode OFF - Continuous animation" << std::endl;
                }
                break;
            case sf::Keyboard::Enter:
                if (stepMode)
                {
                    fastForward = true; // Allow one step to proceed
                    std::cout << "Advancing one step..." << std::endl;
                }
                break;
            case sf::Keyboard::L:
                toggleLabels();
                std::cout << "Labels " << (showLabels ? "enabled" : "disabled") << std::endl;
                break;
            case sf::Keyboard::Equal:
            case sf::Keyboard::Add:
                animationSpeed = std::max(10, animationSpeed - 20);
                std::cout << "Speed increased (delay: " << animationSpeed << "ms)" << std::endl;
                break;
            case sf::Keyboard::Dash:
            case sf::Keyboard::Subtract:
                animationSpeed = std::min(1000, animationSpeed + 20);
                std::cout << "Speed decreased (delay: " << animationSpeed << "ms)" << std::endl;
                break;
            case sf::Keyboard::H:
                std::cout << "\n=== VISUALIZATION CONTROLS ===" << std::endl;
                std::cout << "ESC: Close window" << std::endl;
                std::cout << "SPACE: Toggle Fast forward/Normal speed" << std::endl;
                std::cout << "S: Toggle Step Mode" << std::endl;
                std::cout << "ENTER: Next step (in step mode)" << std::endl;
                std::cout << "L: Toggle g+h cost labels" << std::endl;
                std::cout << "+/-: Increase/Decrease animation speed" << std::endl;
                std::cout << "H: Show this help" << std::endl;
                break;
            }
        }
    }
}

void SFMLVisualizer::drawHelpText()
{
    if (!font.getInfo().family.empty())
    {
        std::string helpText = "Controls: SPACE=Fast forward, S=Step Mode, L=Labels, +/-=Speed, H=Help, ESC=Exit";
        drawText(helpText, sf::Vector2f(10, 10), 12, sf::Color::Black);

        if (algorithmFinished)
        {
            drawText("Algorithm finished! Final path shown in green. Press ESC to exit.",
                     sf::Vector2f(10, 30), 14, sf::Color::Red);
        }
        else if (stepMode)
        {
            if (!fastForward)
            {
                drawText("STEP MODE - Press ENTER for next step", sf::Vector2f(10, 30), 14, sf::Color::Blue);
            }
            else
            {
                drawText("STEP MODE - Processing step...", sf::Vector2f(10, 30), 14, sf::Color::Green);
            }
        }
        else if (!fastForward)
        {
            drawText("FAST FORWARD ON - Press SPACE for normal speed", sf::Vector2f(10, 30), 14, sf::Color::Blue);
        }
        else
        {
            drawText("Normal speed - Press SPACE to fast forward", sf::Vector2f(10, 30), 14, sf::Color::Blue);
        }
    }
}

// Add this method to your SFMLVisualizer class (if it doesn't exist):
bool SFMLVisualizer::shouldProceed() const
{
    if (stepMode)
    {
        // In step mode, only proceed if not paused
        return fastForward;
    }
    else
    {
        // In normal mode, proceed based on fast forward setting
        return true; // Always proceed in normal mode
    }
}

void SFMLVisualizer::draw()
{
    handleEvents();

    // Clear window and prepare for drawing
    window.clear(sf::Color::White);
    drawHelpText();
    window.display();

    // Control animation speed and step mode
    if (!algorithmFinished)
    {
        if (stepMode)
        {
            // Step mode: only sleep and advance if we're allowed to proceed
            if (shouldProceed())
            {
                sf::sleep(sf::milliseconds(animationSpeed));
                fastForward = false; // Pause after this step
            }
        }
        else
        {
            // Normal mode: use fast forward setting
            int delay = !fastForward ? 10 : animationSpeed; // Fast forward = 10ms delay
            sf::sleep(sf::milliseconds(delay));
        }
    }
}


// GraphVisualizer interface implementation
void SFMLVisualizer::markVertex(VertexT vertex, VertexStatus status)
{
    vertexData[vertex].status = status;
}

void SFMLVisualizer::markEdge(EdgeT e, EdgeStatus status)
{
    std::size_t key = getEdgeKey(e.first, e.second);
    edgeData[key].status = status;
    edgeData[key].from = e.first;
    edgeData[key].to = e.second;
}

void SFMLVisualizer::updateVertex(VertexT vertex, double cost, double estimate,
                                  VertexT parent, VertexStatus status)
{
    auto &data = vertexData[vertex];
    data.status = status;
    data.gCost = cost;
    data.hCost = estimate;
    data.parent = parent;
}

// Enhanced visualization methods
void SFMLVisualizer::setCurrentActiveVertex(VertexT vertex)
{
    // Clear previous active state
    if (currentActiveVertex != undefinedVertex)
    {
        vertexData[currentActiveVertex].isCurrentlyActive = false;
    }
    // Set new active vertex
    currentActiveVertex = vertex;
    if (vertex != undefinedVertex)
    {
        vertexData[vertex].isCurrentlyActive = true;
    }
}

void SFMLVisualizer::clearCurrentNeighbors()
{
    for (VertexT neighbor : currentNeighbors)
    {
        vertexData[neighbor].isNeighborInQueue = false;
    }
    currentNeighbors.clear();
}

void SFMLVisualizer::addCurrentNeighbor(VertexT vertex)
{
    currentNeighbors.insert(vertex);
    vertexData[vertex].isNeighborInQueue = true;
}

void SFMLVisualizer::setFinalPath(const std::list<VertexT> &path)
{
    finalPath = path;
}

void SFMLVisualizer::markAlgorithmFinished()
{
    algorithmFinished = true;
    setCurrentActiveVertex(undefinedVertex);
    clearCurrentNeighbors();
}

void SFMLVisualizer::setEdgeWeight(VertexT from, VertexT to, CostT weight)
{
    std::size_t key = getEdgeKey(from, to);
    edgeData[key].weight = weight;
    edgeData[key].from = from;
    edgeData[key].to = to;
}

void SFMLVisualizer::markOptimalPathEdges(const std::list<VertexT> &path)
{
    auto it = path.begin();
    while (it != path.end())
    {
        auto next = std::next(it);
        if (next != path.end())
        {
            std::size_t key = getEdgeKey(*it, *next);
            if (edgeData.find(key) != edgeData.end())
            {
                edgeData[key].isOnOptimalPath = true;
                edgeData[key].status = EdgeStatus::Optimal;
            }
        }
        ++it;
    }
}

void SFMLVisualizer::toggleLabels()
{
    showLabels = !showLabels;
    for (auto &pair : vertexData)
    {
        pair.second.showLabels = showLabels;
    }
}


// ============================================================================
// MazeVisualizer Implementation
// ============================================================================

MazeVisualizer::MazeVisualizer(int windowWidth, int windowHeight,
                               std::size_t mazeW, std::size_t mazeH,
                               const std::vector<std::vector<bool>> &passable,
                               VertexT start, VertexT goal)
    : SFMLVisualizer(windowWidth, windowHeight, "Maze A* Visualization"),
      mazeWidth(mazeW), mazeHeight(mazeH), passableAreas(passable),
      startVertex(start), goalVertex(goal)
{

    // Calculate optimal cell size
    cellSize = std::min(static_cast<float>(windowWidth - 40) / mazeWidth,
                        static_cast<float>(windowHeight - 80) / mazeHeight);

    std::cout << "MazeVisualizer: " << mazeWidth << "x" << mazeHeight
              << ", cellSize=" << cellSize
              << ", start=" << startVertex << ", goal=" << goalVertex << std::endl;

    // Initialize all vertex positions
    for (std::size_t row = 0; row < mazeHeight; ++row)
    {
        for (std::size_t col = 0; col < mazeWidth; ++col)
        {
            VertexT vertex = row * mazeWidth + col;
            vertexData[vertex].position = mazeCoordToScreen(row, col);
        }
    }
}

sf::Vector2i MazeVisualizer::vertexToMazeCoord(VertexT vertex)
{
    return sf::Vector2i(vertex % mazeWidth, vertex / mazeWidth);
}

sf::Vector2f MazeVisualizer::mazeCoordToScreen(int row, int col)
{
    float offsetX = 20; // Leave margin for help text
    float offsetY = 60; // Leave margin for help text
    return sf::Vector2f(offsetX + col * cellSize, offsetY + row * cellSize);
}

void MazeVisualizer::drawMazeContent()
{
    // Create set of final path vertices for highlighting
    std::unordered_set<VertexT> pathVertices;
    for (VertexT vertex : finalPath)
    {
        pathVertices.insert(vertex);
    }

    // Draw maze grid
    for (std::size_t row = 0; row < mazeHeight; ++row)
    {
        for (std::size_t col = 0; col < mazeWidth; ++col)
        {
            VertexT vertex = row * mazeWidth + col;

            sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
            cell.setPosition(mazeCoordToScreen(row, col));

            if (!passableAreas[row][col])
            {
                // Wall - solid black (matching PDF image)
                cell.setFillColor(sf::Color::Black);
            }
            else
            {
                // Passable area - determine color
                sf::Color cellColor = sf::Color::White; // Default

                // Special vertex highlighting
                if (vertex == startVertex)
                {
                    cellColor = sf::Color(0, 255, 255); // Cyan for start
                }
                else if (vertex == goalVertex)
                {
                    cellColor = sf::Color(255, 0, 255); // Magenta for goal
                }
                else if (algorithmFinished && pathVertices.count(vertex))
                {
                    cellColor = sf::Color(0, 255, 0); // Bright green for optimal path
                }
                else
                {
                    // Use A* algorithm status colors
                    auto it = vertexData.find(vertex);
                    if (it != vertexData.end())
                    {
                        cellColor = getVertexColor(it->second);
                    }
                }

                cell.setFillColor(cellColor);
            }

            // Add thin border for better visibility
            cell.setOutlineThickness(0.5f);
            cell.setOutlineColor(sf::Color(128, 128, 128));
            window.draw(cell);

            // Draw g+h cost labels if enabled and space allows
            if (passableAreas[row][col] && showLabels && cellSize > 20)
            {
                auto it = vertexData.find(vertex);
                if (it != vertexData.end() && it->second.status != VertexStatus::UnknownVertex)
                {
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(0)
                       << it->second.gCost << "+" << it->second.hCost;

                    sf::Vector2f textPos = mazeCoordToScreen(row, col);
                    textPos.x += cellSize / 4;
                    textPos.y += cellSize / 4;
                    drawText(ss.str(), textPos, std::max(8, (int)(cellSize / 3)), sf::Color::Black);
                }
            }
        }
    }

    // Draw legend
    drawText("Black=Wall, Cyan=Start, Magenta=Goal, Green=Path",
             sf::Vector2f(20, window.getSize().y - 40), 12, sf::Color::Black);
}

void MazeVisualizer::draw() {
    handleEvents();
    
    // Clear and draw current state
    window.clear(sf::Color::White);
    drawMazeContent();
    drawHelpText();
    window.display();
    
    // Control animation speed and step mode
    if (!algorithmFinished) {
        if (stepMode) {
            // Step mode: only sleep and advance if we're allowed to proceed
            if (shouldProceed()) {
                sf::sleep(sf::milliseconds(animationSpeed));
                fastForward = false; // Pause after this step
            }
        } else {
            // Normal mode: use fast forward setting
            int delay = !fastForward ? 10 : animationSpeed; // Fast forward = 10ms delay
            sf::sleep(sf::milliseconds(delay));
        }
    }
}

// ============================================================================
// RouteGraphVisualizer Implementation
// ============================================================================

RouteGraphVisualizer::RouteGraphVisualizer(int windowWidth, int windowHeight,
                                           const std::vector<std::pair<double, double>> &coordinates)
    : SFMLVisualizer(windowWidth, windowHeight, "Route Graph A* Visualization"),
      nodePositions(coordinates.size())
{

    if (coordinates.empty())
    {
        std::cerr << "Error: No coordinates provided to RouteGraphVisualizer" << std::endl;
        return;
    }

    std::cout << "RouteGraphVisualizer: Loading " << coordinates.size() << " nodes" << std::endl;

    // Find coordinate bounds
    minCoord = maxCoord = sf::Vector2f(coordinates[0].first, coordinates[0].second);
    for (const auto &coord : coordinates)
    {
        minCoord.x = std::min(minCoord.x, static_cast<float>(coord.first));
        minCoord.y = std::min(minCoord.y, static_cast<float>(coord.second));
        maxCoord.x = std::max(maxCoord.x, static_cast<float>(coord.first));
        maxCoord.y = std::max(maxCoord.y, static_cast<float>(coord.second));
    }

    // Calculate scale factor with margins
    float marginX = windowWidth * 0.1f;
    float marginY = windowHeight * 0.15f; // Extra margin for help text
    float scaleX = (windowWidth - 2 * marginX) / (maxCoord.x - minCoord.x);
    float scaleY = (windowHeight - 2 * marginY) / (maxCoord.y - minCoord.y);
    scaleFactor = std::min(scaleX, scaleY);

    std::cout << "Bounds: (" << minCoord.x << "," << minCoord.y
              << ") to (" << maxCoord.x << "," << maxCoord.y
              << "), scale=" << scaleFactor << std::endl;

    // Pre-calculate screen positions
    for (std::size_t i = 0; i < coordinates.size(); ++i)
    {
        nodePositions[i] = graphCoordToScreen(coordinates[i].first, coordinates[i].second);
        vertexData[i].position = nodePositions[i];
    }
}

sf::Vector2f RouteGraphVisualizer::graphCoordToScreen(double x, double y)
{
    float marginX = window.getSize().x * 0.1f;
    float marginY = window.getSize().y * 0.15f;

    float screenX = marginX + (x - minCoord.x) * scaleFactor;
    // Flip Y coordinate (screen Y increases downward, but geographic coordinates increase upward)
    float screenY = window.getSize().y - marginY - (y - minCoord.y) * scaleFactor;

    return sf::Vector2f(screenX, screenY);
}

void RouteGraphVisualizer::drawArrow(sf::Vector2f from, sf::Vector2f to, sf::Color color, float thickness)
{
    sf::Vector2f direction = to - from;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < 2.0f)
        return; // Too short

    direction /= length;

    // Shorten line to avoid overlap with node circles
    float nodeRadius = 10.0f;
    sf::Vector2f adjustedFrom = from + direction * nodeRadius;
    sf::Vector2f adjustedTo = to - direction * nodeRadius;

    // Draw main line as thick rectangle
    sf::Vector2f lineVec = adjustedTo - adjustedFrom;
    float lineLength = std::sqrt(lineVec.x * lineVec.x + lineVec.y * lineVec.y);

    sf::RectangleShape line;
    line.setSize(sf::Vector2f(lineLength, thickness));
    line.setFillColor(color);
    line.setPosition(adjustedFrom);
    line.setOrigin(0, thickness / 2);

    float angle = std::atan2(lineVec.y, lineVec.x) * 180.0f / 3.14159f;
    line.setRotation(angle);
    window.draw(line);

    // Draw arrowhead
    float arrowSize = 12.0f;
    sf::Vector2f perpendicular(-direction.y, direction.x);

    sf::ConvexShape arrow;
    arrow.setPointCount(3);
    arrow.setPoint(0, adjustedTo);
    arrow.setPoint(1, adjustedTo - direction * arrowSize + perpendicular * (arrowSize * 0.4f));
    arrow.setPoint(2, adjustedTo - direction * arrowSize - perpendicular * (arrowSize * 0.4f));
    arrow.setFillColor(color);
    window.draw(arrow);
}

void RouteGraphVisualizer::drawEdges()
{
    // Draw edges in order: unknown, visited, active, optimal
    std::vector<EdgeStatus> drawOrder = {EdgeStatus::UnknownEdge, EdgeStatus::Visited,
                                         EdgeStatus::Active, EdgeStatus::Optimal};

    for (EdgeStatus status : drawOrder)
    {
        for (const auto &edgePair : edgeData)
        {
            const EdgeData &edge = edgePair.second;

            if (edge.status != status)
                continue;

            if (edge.from < nodePositions.size() && edge.to < nodePositions.size())
            {
                sf::Vector2f fromPos = nodePositions[edge.from];
                sf::Vector2f toPos = nodePositions[edge.to];

                sf::Color edgeColor = getEdgeColor(edge.status);
                float thickness = (edge.status == EdgeStatus::Active || edge.status == EdgeStatus::Optimal) ? 4.0f : 2.0f;

                // Special highlighting for optimal path
                if (edge.isOnOptimalPath)
                {
                    edgeColor = sf::Color(0, 200, 0); // Bright green
                    thickness = 5.0f;
                }

                drawArrow(fromPos, toPos, edgeColor, thickness);

                // Draw edge weight (matching PDF style)
                if (edge.weight > 0.0)
                {
                    sf::Vector2f labelPos = fromPos * 0.2f + toPos * 0.8f;
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(1) << edge.weight;

                    // Offset text to avoid overlap with edge
                    sf::Vector2f direction = toPos - fromPos;
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    if (length > 0)
                    {
                        direction /= length;
                        sf::Vector2f perpendicular(-direction.y, direction.x);
                        labelPos += perpendicular * 15.0f;
                    }

                    drawText(ss.str(), labelPos, 10, sf::Color::Black);
                }
            }
        }
    }
}

void RouteGraphVisualizer::drawNodes()
{
    // Create set of final path vertices
    std::unordered_set<VertexT> pathVertices;
    for (VertexT vertex : finalPath)
    {
        pathVertices.insert(vertex);
    }

    // Draw all nodes
    for (std::size_t vertex = 0; vertex < nodePositions.size(); ++vertex)
    {
        sf::Vector2f pos = nodePositions[vertex];

        // Determine node appearance
        auto it = vertexData.find(vertex);
        sf::Color nodeColor = sf::Color(220, 220, 220); // Light gray default
        if (it != vertexData.end())
        {
            nodeColor = getVertexColor(it->second);
        }

        // Highlight path nodes
        if (algorithmFinished && pathVertices.count(vertex))
        {
            nodeColor = sf::Color(0, 255, 0); // Bright green for path
        }

        // Draw node circle
        float radius = 10.0f;
        sf::CircleShape node(radius);
        node.setFillColor(nodeColor);
        node.setOutlineThickness(2.0f);
        node.setOutlineColor(sf::Color::Black);
        node.setOrigin(radius, radius);
        node.setPosition(pos);
        window.draw(node);

        // Draw vertex ID (always visible, matching PDF)
        std::stringstream idss;
        idss << vertex;
        sf::Vector2f idPos = pos;
        idPos.x -= 8;
        idPos.y += 15;
        drawText(idss.str(), idPos, 12, sf::Color::Black);

        // Draw g+h cost labels if enabled (matching PDF format)
        if (it != vertexData.end() && showLabels &&
            it->second.status != VertexStatus::UnknownVertex)
        {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(1)
               << it->second.gCost << "+" << it->second.hCost;

            sf::Vector2f textPos = pos;
            textPos.x += 15;
            textPos.y -= 10;
            drawText(ss.str(), textPos, 10, sf::Color::Blue);
        }
    }
}

void RouteGraphVisualizer::draw()
{
    handleEvents();

    // Clear and draw current state
    window.clear(sf::Color::White);
    drawEdges();
    drawNodes();
    drawHelpText();
    window.display();

    // Control animation speed
    if (!algorithmFinished && shouldProceed())
    {
        sf::sleep(sf::milliseconds(animationSpeed));
        if (stepMode)
        {
            fastForward = false;
        }
    }
}