#include <forest/tree/nodes/base.h>

#include <cassert>
#include <sstream>

Node::Node(Node* parent, Vec2f position, double angle, double length, const Phenotype::SPtr& phenotype)
    : _parent(parent)
    , _depth(parent ? parent->GetDepth() : 0)
    , _position(position)
    , _angle(angle)
    , _length(length)
    , _phenotype(phenotype)
{}

Vec2f Node::GetEdge() const {
    Vec2f edge = _position + Vec2f{1, 0}.rotated(_angle) * _length;
    return edge;
}

size_t Node::GetDepth() const {
    return _depth;
}

Vec2f Node::GetPosition() const {
    return _position;
}

double Node::GetSubtreeMaintenanceConsumption() const {
    return _subtreeMaintenanceConsumption;
}

double Node::_CollectFoodDfs() {
    double food = CollectFood();
    for (SPtr& child : _children) {
        food += child->_CollectFoodDfs();
    }
    return food;
}

void Node::_TickDfs(double food, double elapsedSec) {
    _Tick(food, elapsedSec);

    assert(food > -ExtMath::EPS);

    double foodPart = food / _children.size();

    size_t childrenToTick = _children.size();

    for (size_t i = 0; i < childrenToTick; ++i) {
        _children[i]->_TickDfs(foodPart, elapsedSec);
    }
}

void Node::_RenderDfs(const Renderer::SPtr& renderer) const {
    Render(renderer);

    for (const SPtr& child : _children) {
        child->_RenderDfs(renderer);
    }
}

void Node::AddChild(const SPtr& child) {
    _children.push_back(child);
}

void Node::_Update(World* /*world*/) {
    if (_parent) {
        _position = _parent->GetEdge();
    }
}

void Node::_UpdateDfs(World* world) {
    _Update(world);

    _maintenanceConsumption = _GetMaintenanceConsumption();
    _subtreeMaintenanceConsumption = _maintenanceConsumption;

    for (Node::SPtr& child : _children) {
        child->_UpdateDfs(world);
        _subtreeMaintenanceConsumption += child->GetSubtreeMaintenanceConsumption();
    }
}

const Phenotype::TraitAccessor& Node::_AccessTraits() const {
    return _phenotype->AccessTraits(_depth);
}

std::string Node::ToString() const {
    std::stringstream str;
    Print(str);
    return str.str();
}

void Node::Print(std::ostream& out) const {
    out << "Node {}";
}

void Node::PrintSubtree(std::ostream& out, const std::string& prefix) const {
    out << prefix;
    Print(out);
    out << " _position={" << _position << "}" << std::endl;
    if (_parent) {
        out << "^ ";
        _parent->Print(out);
        out << "edge={" << _parent->GetEdge() << "}" << std::endl;
    }
    for (const Node::SPtr& child : _children) {
        child->PrintSubtree(out, prefix + "-");
    }
}