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

double Node::_CollectFoodSubtree() {
    double food = CollectFood();
    for (SPtr& child : _children) {
        food += child->_CollectFoodSubtree();
    }
    return food;
}

void Node::_TickSubtree(double food, double elapsedSec, World* world) {
    _Tick(food, elapsedSec, world);

    assert(food > -ExtMath::EPS);

    size_t childrenToTick = _children.size();

    for (const Node::SPtr& child : _children) {
        double foodPart = food * child->GetSubtreeMaintenanceConsumption() / _subtreeMaintenanceConsumption;
        child->_TickSubtree(foodPart, elapsedSec, world);
    }
}

void Node::_RenderSubtree(const Renderer::SPtr& renderer) const {
    Render(renderer);

    for (const SPtr& child : _children) {
        child->_RenderSubtree(renderer);
    }
}

void Node::AddChild(const SPtr& child) {
    _newbornChildren.push_back(child);
}

bool Node::_Update(World* /*world*/) {
    if (_parent) {
        _position = _parent->GetEdge();
    }
    return true;
}

bool Node::_UpdateSubtree(World* world) {
    if (!_Update(world)) {
        return false;
    }

    _maintenanceConsumption = _GetMaintenanceConsumption();
    _subtreeMaintenanceConsumption = _maintenanceConsumption;

    std::move(_newbornChildren.begin(), _newbornChildren.end(), std::back_inserter(_children));
    _newbornChildren.clear();

    for (auto it = _children.begin(); it != _children.end(); ++it) {
        if (!(*it)->_UpdateSubtree(world)) {
            it->reset();
        } else {
            _subtreeMaintenanceConsumption += (*it)->GetSubtreeMaintenanceConsumption();
        }
    }

    _children.erase(
        std::remove_if(
            _children.begin(),
            _children.end(),
            [](const Node::SPtr& p) { return !p; }
        ),
        _children.end()
    );
    return true;
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