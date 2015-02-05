/**
 * @file graph.h
 *
 * @brief
 * This file exports a parameterized Graph class used
 * to represent <b><i>graphs,</i></b> which consist of a set of
 * <b><i>nodes</i></b> (vertices) and a set of <b><i>arcs</i></b> (edges).
 * 
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

#ifndef _graph_h
#define _graph_h

#include <string>
#include "compare.h"
#include "error.h"
#include "hashcode.h"
#include "map.h"
#include "set.h"
#include "tokenscanner.h"
#include "strlib.h"

/**
 * @class Graph
 *
 * @brief This class represents a graph with the specified node and arc types.
 *
 * The \c NodeType and \c ArcType parameters indicate
 * the structure type or class used for nodes and arcs, respectively.
 * These types can contain any fields or methods required by the client,
 * but must contain the following fields required by the `%Graph`
 * package itself:
 *
 * The \c NodeType definition must include:
 * <ul>
 *   <li>A \c string field called \c name
 *   <li>A `Set<ArcType *>` field called \c arcs
 * </ul>
 *
 * The \c ArcType definition must include:
 * <ul>
 *   <li>A `NodeType *` field called \c start
 *   <li>A `NodeType *` field called \c finish
 * </ul>
 *
 * As a convenience, this class provides methods that support reading graph data
 * in a simple textual form from an input stream (or string) via a TokenScanner,
 * and methods that support writing graph data to an output stream.
 */

template <typename NodeType, typename ArcType>
class Graph {
public:
/**
 * Creates an empty `%Graph` object.
 *
 * Sample usage:
 *
 *     Graph<NodeType,ArcType> g;
 */
    Graph();
    
/**
 * Frees the internal storage allocated to represent the graph.
 */
    virtual ~Graph();

/** \_overload */
   ArcType* addArc(const std::string& s1, const std::string& s2);
/** \_overload */
   ArcType* addArc(NodeType* n1, NodeType* n2);
/**
 * Adds an arc to this graph.  The endpoints of the arc can be specified
 * either as strings indicating the names of the nodes or as pointers
 * to the node structures.  Alternatively, the client can create the arc
 * structure explicitly and pass that pointer to the `%addArc`
 * method.  All three of these versions return a pointer to the arc in
 * case the client needs to capture this value.
 * If the third form is called and the start/finish nodes passed are not
 * already part of the graph, they are added to the graph.
 * If any pointer passed is \c NULL, this method signals an error.
 *
 * Sample usages:
 *
 *     g.addArc(s1, s2);
 *     g.addArc(n1, n2);
 *     g.addArc(arc);
 */
    ArcType* addArc(ArcType* arc);


/** \_overload */
   NodeType* addNode(const std::string& name);
/**
 * Adds a node to this graph.  The first version of this method
 * creates a new node of the appropriate type and initializes its
 * fields; the second assumes that the client has already created
 * the node and simply adds it to the graph.  Both versions of this
 * method return a pointer to the node.
 * If any pointer passed is \c NULL, this method signals an error.
 *
 * Sample usages:
 *
 *     NodeType* node = g.addNode(name);
 *     NodeType* node = g.addNode(node);
 */
    NodeType* addNode(NodeType* node);


/**
 * Reinitializes this graph to be empty, freeing any heap storage.
 *
 * Sample usage:
 *
 *     g.clear();
 */
    void clear();
    

/**
 * Compares two graphs for equality.
 * Returns \c true if this graph contains exactly the same
 * vertices, edges, and connections as the given other graph.
 * Identical in behavior to the \c == operator.
 *
 * Sample usage:
 *
 *     if (g.equals(graph2)) ...
 */
    bool equals(const Graph<NodeType, ArcType>& graph2) const;
    
    const Set<ArcType*>& getArcSet(NodeType* node) const;
    const Set<ArcType*>& getArcSet(const std::string& name) const;
/**
 * Returns the set of all arcs in this graph or, in the second and
 * third forms, the arcs that start at the specified node, which
 * can be indicated either as a pointer or by name.
 * If any pointer passed is \c NULL, throws an error.
 *
 * Sample usages:
 *
 *     for (ArcType* arc : g.getArcSet()) ...
 *     for (ArcType* arc : g.getArcSet(node)) ...
 *     for (ArcType* arc : g.getArcSet(name)) ...
 */
    const Set<ArcType*>& getArcSet() const;


/** \_overload */
   const Set<NodeType*> getNeighbors(NodeType* node) const;
/**
 * Returns the set of nodes that are neighbors of the specified
 * node, which can be indicated either as a pointer or by name.
 * If any pointer passed is \c NULL, or if the given node is not found
 * in this graph, throws an error.
 *
 * Sample usages:
 *
 *     for (NodeType *node : g.getNeighbors(node)) ...
 *     for (NodeType *node : g.getNeighbors(name)) ...
 */
   const Set<NodeType*> getNeighbors(const std::string& node) const;


/**
 * Looks up a node in the name table attached to this graph and
 * returns a pointer to that node.  If no node with the specified
 * name exists, returns \c NULL.
 *
 * Sample usage:
 *
 *     NodeType* node = g.getNode(name);
 */
   NodeType* getNode(const std::string& name) const;
    

/**
 * Returns the set of all nodes in this graph.
 *
 * Sample usage:
 *
 *     for (NodeType *node : g.getNodeSet()) ...
 */
    const Set<NodeType*>& getNodeSet() const;


/** \_overload */
   bool isConnected(NodeType* n1, NodeType* n2) const;
/**
 * Returns \c true if this graph contains an arc from
 * \em n1 to \em n2</code>.  As in the \ref addArc
 * method, nodes can be specified either as node pointers or by name.
 * If any pointer passed is \c NULL, or if either node is not contained
 * in this graph, returns false.
 *
 * Sample usages:
 *
 *     if (g.isConnected(n1, n2)) ...
 *     if (g.isConnected(s1, s2)) ...
 */
    bool isConnected(const std::string& s1, const std::string& s2) const;


/**
 * Returns \c true if this graph is empty.
 *
 * Sample usage:
 *
 *     if (g.isEmpty()) ...
 */
    bool isEmpty() const;


/** \_overload */
   void removeArc(NodeType* n1, NodeType* n2);
/** \_overload */
   void removeArc(ArcType* arc);
/**
 * Removes an arc from this graph. The arc can be specified in any
 * of three ways: by the names of its endpoints, by the node pointers
 * at its endpoints, or as an arc pointer.  If more than one arc
 * connects the specified endpoints, all of them are removed.
 * If no arc connects the given endpoints, or the given arc is not found,
 * this method has no effect.
 *
 * Sample usages:
 *
 *     g.removeArc(s1, s2);
 *     g.removeArc(n1, n2);
 *     g.removeArc(arc);
 */
   void removeArc(const std::string& s1, const std::string& s2);


/** \_overload */
   void removeNode(const std::string& name);
/**
 * Removes a node from this graph. The node can be specified
 * either by its name or as a pointer value.  Removing a node also
 * removes all arcs that contain that node.
 * If a node or name is passed that is not part of the graph,
 * this method has no effect.
 *
 * Sample usages:
 *
 *     g.removeNode(name);
 *     g.removeNode(node);
 */
   void removeNode(NodeType* node);


/**
 * Reads the data for an arc from the scanner.  The \em forward
 * argument points to the arc in the forward direction.  If the arc is
 * undirected, \em backward points to the reverse arc; for
 * directed arcs, the \em backward pointer is \c NULL.
 * The default implementation of this method is empty.  Clients that want
 * to initialize other fields in the arc must override this method so
 * that it initializes one or both arcs, as appropriate.
 *
 * Sample usage:
 *
 *     scanArcData(scanner, forward, backward);
 */
    virtual void scanArcData(TokenScanner& scanner,
                             ArcType* forward, ArcType* backward) {
        /* Empty */
    }


/**
 * Reads one "entry" for the graph from a scanner. An entry
 * is either a node description or an arc description.  This
 * method returns \c true if it reads an entry, and
 * \c false at the end of file or at text that cannot be
 * recognized as a graph entry.
 *
 * A node description consists of the name of the node (which may be quoted
 * if it contains special characters), optionally followed by data for
 * the node.  An arc description has one of the following forms:
 *
 *       n1 -> n2
 *       n1 - n2
 *
 * either of which can be followed by data for the arc.  The first form
 * creates a single directed arc; the second creates two arcs, one in
 * each direction.
 *
 * Clients who want to read node or arc data must override the empty
 * versions of \ref scanNodeData and \ref scanArcData
 * defined in this interface.
 *
 * Sample usage:
 *
 *     while (g.scanGraphEntry(scanner)) { }
 */
    virtual bool scanGraphEntry(TokenScanner& scanner);


/**
 * Reads the data for a node from a scanner.  The default
 * implementation of this method is empty.  Clients that want to initialize
 * other fields in the node from the token stream must override this method.
 *
 * Sample usage:
 *
 *     scanNodeData(scanner, node);
 */
    virtual void scanNodeData(TokenScanner& scanner, NodeType* node) {
        /* Empty */
    }


/**
 * Returns the number of nodes in this graph.
 *
 * Sample usage:
 *
 *     int size = g.size();
 */
    int size() const;


/**
 * Returns a printable string representation of this graph.
 *
 * Sample usage:
 *
 *     string str = g.toString();
 */
    std::string toString() const;
    

/**
 * Writes the data for an arc to an output stream.  The default
 * implementation of this method is empty.  Clients that want to store
 * other fields from the arc must override this method so that it writes
 * that data in a form that \ref scanArcData can read.
 *
 * Sample usage:
 *
 *     writeArcData(os, arc);
 */
    virtual void writeArcData(std::ostream& os, ArcType* arc) const {
        /* Empty */
    }


/**
 * Writes the data for a node to an output stream.  The default
 * implementation of this method is empty.  Clients that want to store
 * other fields from the node must override this method so that it
 * writes that data in a form that \ref scanNodeData can read.
 *
 * Sample usage:
 *
 *     writeNodeData(os, node);
 */
    virtual void writeNodeData(std::ostream& os, NodeType* node) const {
        /* Empty */
    }


 /*
 * Iterator support
 * ----------------
 * The classes in the StanfordCPPLib collection implement input
 * iterators so that they work symmetrically with respect to the
 * corresponding STL classes.
 */
    class graph_iterator : public std::iterator<std::input_iterator_tag, NodeType*> {
    public:
        graph_iterator() : m_graph(NULL) {
            // empty
        }

        graph_iterator(const Graph& graph, bool end) {
            m_graph = &graph;
            if (end) {
                m_itr = m_graph->getNodeSet().end();
            } else {
                m_itr = m_graph->getNodeSet().begin();
            }
        }

        graph_iterator(const graph_iterator& it)
                : m_graph(it.m_graph), m_itr(it.m_itr) {
            // empty
        }

        graph_iterator& operator ++() {
            m_itr++;
            return *this;
        }

        graph_iterator operator ++(int) {
            graph_iterator copy(*this);
            operator++();
            return copy;
        }

        bool operator ==(const graph_iterator& rhs) {
            return m_graph == rhs.m_graph && m_itr == rhs.m_itr;
        }

        bool operator !=(const graph_iterator& rhs) {
            return !(*this == rhs);
        }

        NodeType* operator *() {
            return *m_itr;
        }

        NodeType** operator ->() {
            return &(*m_itr);
        }

        friend class Graph;
        
    private:
        const Graph* m_graph;
        typename Set<NodeType*>::iterator m_itr;
    };
    
    graph_iterator begin() const {
        return graph_iterator(*this, /* end */ false);
    }

    graph_iterator end() const {
        return graph_iterator(*this, /* end */ true);
    }
    

    /** \_overload */ // not an overload; just a hack for Doxygen
    bool operator !=(const Graph& graph2) const;
/**
 * Relational operators to compare two graphs for equality
 * or inequality.
 *
 * Sample usages:
 *
 *      if (g1 == g2) ...
 *      if (g1 != g2) ...
 */
// * The <, >, <=, >= operators require that the ValueType has a < operator
// * so that the elements can be compared pairwise.
// */
    bool operator ==(const Graph& graph2) const;
//    bool operator <(const Graph& graph2) const;
//    bool operator <=(const Graph& graph2) const;
//    bool operator >(const Graph& graph2) const;
//    bool operator >=(const Graph& graph2) const;

    
    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
 * Private class: GraphComparator
 * ------------------------------
 * This template class establishes the ordering for nodes and arcs.
 * Nodes are processed in alphabetical order by node name; arcs are
 * compared in much the same way, looking first at the start node and
 * then continuing on to look at the finish node if the start nodes
 * match.  These functions, however, indicate equality only if the
 * arguments are identical, in the sense that they are at the same
 * address.  If two distinct arcs, for example, connect the same pair
 * of nodes (which is perfectly legal in the graph abstraction and can
 * be used, for example, to represent multiple modes of travel between
 * two nodes), those arcs are not the same.
 */
    class GraphComparator {
    public:
        bool operator()(NodeType* n1, NodeType* n2) {
            return compare(n1, n2) < 0;
        }

        bool operator()(ArcType* a1, ArcType* a2) {
            return compare(a1, a2) < 0;
        }
    };

private:
    /* Instance variables */
    Set<NodeType*> nodes;                  /* The set of nodes in the graph */
    Set<ArcType*> arcs;                    /* The set of arcs in the graph  */
    Map<std::string, NodeType*> nodeMap;   /* A map from names to nodes */
    GraphComparator comparator;            /* The comparator for this graph */

public:
/*
 * Functions: operator=, copy constructor
 * --------------------------------------
 * These functions are part of the public interface of the class but are
 * defined here to avoid adding confusion to the Graph class.
 */
    Graph& operator =(const Graph& src);
    Graph(const Graph& src);

    static int compare(NodeType* n1, NodeType* n2) {
        if (n1 == n2) {
            return 0;
        }
        if (n1->name < n2->name) {
            return -1;
        }
        if (n1->name > n2->name) {
            return +1;
        }
        return (n1 < n2) ? -1 : +1;
    }

    static int compare(ArcType* a1, ArcType* a2) {
        if (a1 == a2) {
            return 0;
        }
        NodeType* n1 = a1->start;
        NodeType* n2 = a2->start;
        if (n1 != n2) {
            return compare(n1, n2);
        }
        n1 = a1->finish;
        n2 = a2->finish;
        if (n1 != n2) {
            return compare(n1, n2);
        }
        return (a1 < a2) ? -1 : +1;
    }

private:
    void deepCopy(const Graph& src);
    NodeType* getExistingNode(const std::string& name, const std::string& member = "") const;
    int graphCompare(const Graph& graph2) const;
    bool isExistingArc(ArcType* arc) const;
    bool isExistingNode(NodeType* node) const;
    void verifyExistingNode(NodeType* node, const std::string& member = "") const;
    void verifyNotNull(void* p, const std::string& member = "") const;
    NodeType* scanNode(TokenScanner& scanner);
};


/**
 * Returns a hash code for the given Graph. This function is provided
 * for convenience so that clients do not need to write their own
 * \c hashCode function in order to be able to create a HashSet of
 * Graph objects or a HashMap whose keys are Graph objects.
 *
 * Sample usage:
 *
 *     int hash = hashCode(graph);
 *
 */
template <typename NodeType, typename ArcType>
int hashCode(const Graph<NodeType, ArcType>& graph);


/*
 * Implementation notes: Graph constructor
 * ---------------------------------------
 * Even though the body of the Graph constructor is empty, important
 * work is done by the initializers, which ensure that the nodes and
 * arcs set are given the correct comparison functions.
 */
template <typename NodeType, typename ArcType>
Graph<NodeType, ArcType>::Graph() {
    comparator = GraphComparator();
    nodes = Set<NodeType*>(comparator);
    arcs = Set<ArcType*>(comparator);
}

template <typename NodeType, typename ArcType>
Graph<NodeType, ArcType>::Graph(const Graph& src) {
    nodes = Set<NodeType*>(comparator);
    arcs = Set<ArcType*>(comparator);
    deepCopy(src);
}

/*
 * Implementation notes: Graph destructor
 * --------------------------------------
 * The destructor must free all heap storage used by this graph to
 * represent the nodes and arcs.  The clear metho must also reclaim
 * this memory, which means that the destructor can simply call
 * clear to do the work.
 */
template <typename NodeType, typename ArcType>
Graph<NodeType, ArcType>::~Graph() {
    clear();
}

/*
 * Implementation notes: addArc
 * ----------------------------
 * The addArc method appears in three forms, as described in the
 * interface.  The code for each form of the method, however, is
 * quite straightforward.
 */
template <typename NodeType, typename ArcType>
ArcType* Graph<NodeType, ArcType>::addArc(const std::string& s1, const std::string& s2) {
    return addArc(getExistingNode(s1, "addArc"), getExistingNode(s2, "addArc"));
}

template <typename NodeType, typename ArcType>
ArcType* Graph<NodeType, ArcType>::addArc(NodeType* n1, NodeType* n2) {
    verifyExistingNode(n1, "addArc");
    verifyExistingNode(n2, "addArc");
    ArcType* arc = new ArcType();
    arc->start = n1;
    arc->finish = n2;
    return addArc(arc);
}

template <typename NodeType, typename ArcType>
ArcType* Graph<NodeType, ArcType>::addArc(ArcType* arc) {
    verifyNotNull(arc, "addArc");
    if (!isExistingNode(arc->start)) {
        addNode(arc->start);
    }
    if (!isExistingNode(arc->finish)) {
        addNode(arc->finish);
    }
    arc->start->arcs.add(arc);
    arcs.add(arc);
    return arc;
}

/*
 * Implementation notes: addNode
 * -----------------------------
 * The addNode method appears in two forms: one that creates a node
 * from its name and one that assumes that the client has created
 * the new node.  In each case, the implementation must add the node
 * the set of nodes for the graph and add the name-to-node association
 * to the node map.
 */
template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::addNode(const std::string& name) {
    if (nodeMap.containsKey(name)) {
        error("Graph::addNode: node " + name + " already exists");
    }
    NodeType* node = new NodeType();
    node->arcs = Set<ArcType*>(comparator);
    node->name = name;
    return addNode(node);
}

template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::addNode(NodeType* node) {
    verifyNotNull(node, "addNode");
    if (nodeMap.containsKey(node->name)) {
        error("Graph::addNode: node " + node->name + " already exists");
    }
    nodes.add(node);
    nodeMap[node->name] = node;
    return node;
}

/*
 * Implementation notes: clear
 * ---------------------------
 * The implementation of clear first frees the nodes and arcs in
 * their respective sets and then uses the Set class clear method
 * to ensure that these sets are empty.
 */
template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::clear() {
    for (NodeType* node : nodes) {
        delete node;
    }
    for (ArcType* arc : arcs) {
        delete arc;
    }
    arcs.clear();
    nodes.clear();
    nodeMap.clear();
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::equals(const Graph<NodeType, ArcType>& graph2) const {
    return *this == graph2;
}

template <typename NodeType, typename ArcType>
const Set<ArcType*>& Graph<NodeType, ArcType>::getArcSet() const {
    return arcs;
}

template <typename NodeType, typename ArcType>
const Set<ArcType*>&
Graph<NodeType, ArcType>::getArcSet(NodeType* node) const {
    verifyExistingNode(node, "getArcSet");
    return node->arcs;
}

template <typename NodeType, typename ArcType>
const Set<ArcType*>&
Graph<NodeType, ArcType>::getArcSet(const std::string& name) const {
    return getArcSet(getExistingNode(name, "getArcSet"));
}

template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::getExistingNode(const std::string& name, const std::string& member) const {
    NodeType* node = nodeMap.get(name);
    if (!node) {
        error("Graph::" + member + ": no node named " + name);
    }
    return node;
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isExistingArc(ArcType* arc) const {
    return arc && arcs.contains(arc);
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isExistingNode(NodeType* node) const {
    return node && nodeMap.containsKey(node->name) && nodeMap.get(node->name) == node;
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::verifyExistingNode(NodeType* node, const std::string& member) const {
    verifyNotNull(node, member);
    if (!isExistingNode(node)) {
        error("Graph::" + member + ": node not found in graph");
    }
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::verifyNotNull(void* p, const std::string& member) const {
    if (!p) {
        error("Graph::" + member + ": parameter cannot be null");
    }
}

/*
 * Implementation notes: getNeighbors
 * ----------------------------------
 * This implementation recomputes the set each time, which is reasonably
 * efficient if the degree of the node is small.
 */
template <typename NodeType, typename ArcType>
const Set<NodeType*>
Graph<NodeType, ArcType>::getNeighbors(NodeType* node) const {
    verifyExistingNode(node, "getNeighbors");
    Set<NodeType*> nodes = Set<NodeType*>(comparator);
    for (ArcType* arc : node->arcs) {
        nodes.add(arc->finish);
    }
    return nodes;
}

template <typename NodeType, typename ArcType>
const Set<NodeType*>
Graph<NodeType, ArcType>::getNeighbors(const std::string& name) const {
    return getNeighbors(getExistingNode(name, "getNeighbors"));
}

/*
 * Implementation notes: getNode, getExistingNode
 * ----------------------------------------------
 * The getNode method simply looks up the name in the map, which correctly
 * returns NULL if the name is not found.  Other methods in the
 * implementation call the private method getExistingNode instead,
 * which checks for a NULL value and signals an error.
 */
template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::getNode(const std::string& name) const {
    return nodeMap.get(name);
}

/*
 * Implementation notes: getNodeSet, getArcSet
 * -------------------------------------------
 * These methods simply return the set requested by the client.  The
 * sets are returned by reference for efficiency, because doing so
 * eliminates the need to copy the set.
 */
template <typename NodeType, typename ArcType>
const Set<NodeType*>& Graph<NodeType, ArcType>::getNodeSet() const {
    return nodes;
}

/*
 * Implementation notes: isConnected
 * ---------------------------------
 * Node n1 is connected to n2 if any of the arcs leaving n1 finish at n2.
 * The two versions of this method allow nodes to be specified either as
 * node pointers or by name.
 */
template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isConnected(NodeType* n1, NodeType* n2) const {
    // don't call verifyExistingNode here because it will throw an error
    // if n1 or n2 is not found; should just make the call return false
    if (!isExistingNode(n1) || !isExistingNode(n2)) {
        return false;
    }
    for (ArcType* arc : n1->arcs) {
        if (arc->finish == n2) {
            return true;
        }
    }
    return false;
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isConnected(const std::string& s1,
                                           const std::string& s2) const {
    // don't call getExistingNode here because it will throw an error
    // if s1 or s2 is not found; should just make the call return false
    return isConnected(nodeMap.get(s1), nodeMap.get(s2));
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::isEmpty() const {
    return nodes.isEmpty();
}

/*
 * Implementation notes: removeArc
 * -------------------------------
 * These methods remove arcs from the graph, which is ordinarily simply
 * a matter of removing the arc from two sets: the set of arcs in the
 * graph as a whole and the set of arcs in the starting node.  The
 * methods that remove an arc specified by its endpoints, however,
 * must take account of the fact that there might be more than one
 * such arc and delete all of them.
 */
template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::removeArc(const std::string& s1, const std::string& s2) {
    // don't call getExistingNode here because it will throw an error
    // if s1 or s2 is not found; should just make the call have no effect
    removeArc(nodeMap.get(s1), nodeMap.get(s2));
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::removeArc(NodeType* n1, NodeType* n2) {
    // don't call verifyExistingNode here because it will throw an error
    // if n1 or n2 is not found; should just make the call have no effect
    if (!isExistingNode(n1) || !isExistingNode(n2)) {
        return;
    }
    Vector<ArcType*> toRemove;
    for (ArcType* arc : arcs) {
        if (arc->start == n1 && arc->finish == n2) {
            toRemove.add(arc);
        }
    }
    for (ArcType* arc : toRemove) {
        removeArc(arc);
    }
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::removeArc(ArcType* arc) {
    if (!isExistingArc(arc)) {
        return;
    }
    arc->start->arcs.remove(arc);
    arcs.remove(arc);
}

/*
 * Implementation notes: removeNode
 * --------------------------------
 * The removeNode method must remove the specified node but must
 * also remove any arcs in the graph containing the node.  To avoid
 * changing the node set during iteration, this implementation creates
 * a vector of arcs that require deletion.
 */
template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::removeNode(const std::string& name) {
    // don't call getExistingNode here because it will throw an error
    // if name is not found; should just make the call have no effect
    removeNode(nodeMap.get(name));
}

template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::removeNode(NodeType* node) {
    // don't call verifyExistingNode here because it will throw an error
    // if node is not found; should just make the call have no effect
    if (!isExistingNode(node)) {
        return;
    }
    Vector<ArcType*> toRemove;
    for (ArcType* arc : arcs) {
        if (arc->start == node || arc->finish == node) {
            toRemove.add(arc);
        }
    }
    for (ArcType* arc : toRemove) {
        removeArc(arc);
    }
    nodes.remove(node);
}

/*
 * Implementation notes: scanGraphEntry
 * ------------------------------------
 * The scanGraphEntry and its helper methods take a scanner that is
 * initialized to the input stream and has the options ignoreWhitespace,
 * scanStrings, and scanNumbers set.
 */
template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::scanGraphEntry(TokenScanner& scanner) {
    NodeType* n1 = scanNode(scanner);
    if (n1 == NULL) {
        return false;
    }
    std::string op = scanner.nextToken();
    if (op != "-" && op != "->") {
        scanner.saveToken(op);
        return true;
    }
    NodeType* n2 = scanNode(scanner);
    if (n2 == NULL) {
        error("Graph::scanGraphEntry: Missing node after " + op);
    }
    ArcType* forward = new ArcType();
    forward->start = n1;
    forward->finish = n2;
    addArc(forward);
    ArcType* backward = NULL;
    if (op == "-") {
        backward = new ArcType();
        backward->start = n2;
        backward->finish = n1;
        addArc(backward);
    }
    scanArcData(scanner, forward, backward);
    return true;
}

template <typename NodeType, typename ArcType>
NodeType* Graph<NodeType, ArcType>::scanNode(TokenScanner& scanner) {
    std::string token = scanner.nextToken();
    switch (scanner.getTokenType(token)) {
    case WORD:
        break;
    case STRING:
        token = scanner.getStringValue(token);
        break;
    default:
        scanner.saveToken(token);
        return NULL;
    }
    NodeType* node = getNode(token);
    if (node == NULL) {
        node = new NodeType();
        node->name = token;
        scanNodeData(scanner, node);
        addNode(node);
    }
    return node;
}

/*
 * Implementation notes: size, isEmpty
 * -----------------------------------
 * These methods are defined in terms of the node set, so the implementation
 * simply forwards the request there.  Note that it is impossible for a
 * graph to have arcs if it has no nodes.
 */
template <typename NodeType, typename ArcType>
int Graph<NodeType, ArcType>::size() const {
    return nodes.size();
}

template <typename NodeType, typename ArcType>
std::string Graph<NodeType, ArcType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

/*
 * Implementation notes: operator =, copy constructor
 * -------------------------------------------------
 * These methods ensure that copying a graph creates an entirely new
 * parallel structure of nodes and arcs.
 */
template <typename NodeType, typename ArcType>
Graph<NodeType,ArcType>&
Graph<NodeType, ArcType>::operator =(const Graph& src) {
    if (this != &src) {
        clear();
        deepCopy(src);
    }
    return *this;
}

/*
 * Private method: deepCopy
 * ------------------------
 * Common code factored out of the copy constructor and operator= to
 * copy the contents from the other graph.
 */
template <typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::deepCopy(const Graph& src) {
    for (NodeType* oldNode : src.nodes) {
        NodeType* newNode = new NodeType();
    *newNode = *oldNode;
        newNode->arcs.clear();
        addNode(newNode);
    }
    for (ArcType* oldArc : src.arcs) {
        ArcType* newArc = new ArcType();
    *newArc = *oldArc;
        newArc->start = getExistingNode(oldArc->start->name, "deepCopy");
        newArc->finish = getExistingNode(oldArc->finish->name, "deepCopy");
        addArc(newArc);
    }
}


/*
 * Compares two graphs for <, <=, ==, !=, >, >= relational operators.
 * Vertices are compared, including their neighboring edges.
 */
template <typename NodeType, typename ArcType>
int Graph<NodeType, ArcType>::graphCompare(const Graph<NodeType, ArcType>& graph2) const {
    // optimization: if literally the same graph, return true
    if (this == &graph2) {
        return 0;
    }
    
    auto itr1 = begin();
    auto itr2 = graph2.begin();
    auto g1end = end();
    auto g2end = graph2.end();
    
    while (itr1 != g1end && itr2 != g2end) {
        // compare each pair of elements from iterators
        NodeType* node1 = *itr1;
        NodeType* node2 = *itr2;
        
        // optimization: if literally same node, equal; don't compare
        if (node1 != node2) {
            // first check names
            if (node1->name != node2->name) {
                return node1->name.compare(node2->name);
            }
            
            // then check all edges, pairwise
            auto eitr1 = node1->arcs.begin();
            auto eitr2 = node2->arcs.begin();
            auto e1end = node1->arcs.end();
            auto e2end = node2->arcs.end();
            while (eitr1 != e1end && eitr2 != e2end) {
                ArcType* arc1 = *eitr1;
                ArcType* arc2 = *eitr2;
                
                // optimization: if literally same edge, equal; don't compare
                if (arc1 != arc2) {
                    // first check start vertex names, then end vertex names
                    if (arc1->start->name != arc2->start->name) {
                        return arc1->start->name.compare(arc2->start->name);
                    } else if (arc1->finish->name != arc2->finish->name) {
                        return arc1->finish->name.compare(arc2->finish->name);
                    }
                }
                eitr1++;
                eitr2++;
            }
            
            // if we get here, everything from me matched graph2, so either edges equal,
            // or one is shorter than the other (fewer edges) and is therefore less
            if (eitr1 == e1end && eitr2 == e2end) {
                // keep going
            } else if (eitr1 == e1end) {
                return -1;
            } else {
                return 1;
            }
        }
        
        // if we get here, those two vertices and their outbound edges
        // were equal; so advance to next element
        itr1++;
        itr2++;
    }
    
    // if we get here, everything from me matched graph2, so either equal,
    // or one is shorter than the other (fewer vertices) and is therefore less
    if (itr1 == g1end && itr2 == g2end) {
        return 0;
    } else if (itr1 == g1end) {
        return -1;
    } else {
        return 1;
    }
}

/*
 * Operators
 */
template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::operator ==(const Graph& graph2) const {
    // optimization: if sizes not same, graphs not equal
    if (nodes.size() != graph2.nodes.size()
            || arcs.size() != graph2.arcs.size()
            || nodeMap.size() != graph2.nodeMap.size()) {
        return false;
    }
    return graphCompare(graph2) == 0;
}

template <typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::operator !=(const Graph& graph2) const {
    return !(*this == graph2);
}

//template <typename NodeType, typename ArcType>
//bool Graph<NodeType, ArcType>::operator <(const Graph& graph2) const {
//    return graphCompare(graph2) < 0;
//}

//template <typename NodeType, typename ArcType>
//bool Graph<NodeType, ArcType>::operator <=(const Graph& graph2) const {
//    return graphCompare(graph2) <= 0;
//}

//template <typename NodeType, typename ArcType>
//bool Graph<NodeType, ArcType>::operator >(const Graph& graph2) const {
//    return graphCompare(graph2) > 0;
//}

//template <typename NodeType, typename ArcType>
//bool Graph<NodeType, ArcType>::operator >=(const Graph& graph2) const {
//    return graphCompare(graph2) >= 0;
//}

/*
 * Implementation notes: << and >>
 * -------------------------------
 * The insertion and extraction operators for graphs are more complicated
 * than for the standard collection types because the nodes and arcs can
 * contain client-specific data.  To ensure that this information is
 * correctly written and read by these operators, clients must override
 * the methods writeNodeData, writeArcData, scanNodeData, and scanArcData.
 */
/**
 * Overloads the `<<` operator so that it is able
 * to display `%Graph` objects.
 *
 * Sample usage:
 *
 *     cout << g;
 */
template <typename NodeType, typename ArcType>
std::ostream& operator <<(std::ostream& os, const Graph<NodeType, ArcType>& g) {
    os << "{";
    bool started = false;
    for (NodeType *node : g.getNodeSet()) {
        if (started) {
            os << ", ";
        }
        writeGenericValue(os, node->name, stringIsInteger(node->name) || stringIsReal(node->name));
        g.writeNodeData(os, node);
        started = true;
    }
    for (ArcType* arc : g.getArcSet()) {
        os << ", ";
        writeGenericValue(os, arc->start->name, stringIsInteger(arc->start->name) || stringIsReal(arc->start->name));
        os << " -> ";
        writeGenericValue(os, arc->finish->name, stringIsInteger(arc->finish->name) || stringIsReal(arc->finish->name));
        g.writeArcData(os, arc);
    }
    return os << "}";
}

template <typename NodeType, typename ArcType>
std::istream& operator >>(std::istream& is, Graph<NodeType, ArcType>& g) {
    TokenScanner scanner(is);
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.scanStrings();
    scanner.addOperator("->");
    std::string token = scanner.nextToken();
    if (token != "{") {
        error("Graph::operator >>: Missing {");
    }
    g.clear();
    while (g.scanGraphEntry(scanner)) {
        token = scanner.nextToken();
        if (token == "}") {
            scanner.saveToken(token);
        } else if (token != ",") {
            error("Graph::operator >>: Unexpected token " + token);
        }
    }
    token = scanner.nextToken();
    if (token != "}") {
        error("Graph::operator >>: Missing }");
    }
    return is;
}

/*
 * Template hash function for graphs.
 */
template <typename NodeType, typename ArcType>
int hashCode(const Graph<NodeType, ArcType>& graph) {
    int code = HASH_SEED;
    for (NodeType* node : graph) {
        code = HASH_MULTIPLIER * code + hashCode(node->name);
    }
    for (ArcType* arc : graph.getArcSet()) {
        code = HASH_MULTIPLIER * code + hashCode(arc->start);
        code = HASH_MULTIPLIER * code + hashCode(arc->finish);
    }
    return (code & HASH_MASK);
}

#endif
