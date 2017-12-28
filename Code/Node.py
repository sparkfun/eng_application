"""
A class used for the RecursiveTrie data structure.  The RecursiveTrie is comprised of instances of the Node class.
"""

class Node:
    """
    Nodes used in RecursiveTrie
    """
    def __init__(self):
        """
        Constructor
        """
        self.value = ''
        self.count = 1
        self.next = None
        self.adjacent = None

    def getNext(self):
        """
        Accessor for the the next node in a linked list.
        :return:
        Node next
        """
        if self.hasNext():
            return self.next
        else:
            return None

    def getAdjacent(self):
        """
        Accessor for the adjacent node in a linked list.
        :return:
        Node adjacent
        """
        return self.adjacent

    def getValue(self):
        """
        Accessor for the value of the node
        :return:
        char value
        """
        return self.value

    def setValue(self, value):
        """"
        Mutator for the value of the node
        """
        if len(value) == 1:
            self.value = value

    def getCount(self):
        """
        Accessor for the count of the node
        :return:
        int count
        """
        return self.count

    def hasNext(self):
        """
        Returns True if the next node is initialized
        :return:
        boolean
        """
        if self.next == None:
            return False
        else:
            return True

    def hasAdjacent(self):
        """
        Returns true if the adjacent node is initialized
        :return:
        """
        if self.adjacent == None:
            return False
        else:
            return True

    def addNext(self, string):
        """
        Recursively add a string to the next node
        :param string:
        The string to be added to next
        """
        if not self.hasNext():
            self.next = Node()
            self.next.setValue(string[:1])
            if len(string) > 1:
                self.next.addNext(string[1:])
        elif self.getNextByValue(string[:1]) != None:
            self.getNextByValue(string[:1]).count += 1
            if len(string) > 1:
                self.next.addNext(string[1:])
        else:
            node = Node()
            node.setValue(string[:1])
            node.adjacent = self.getNext()
            self.next = node
            if len(string) > 1:
                self.next.addNext(string[1:])

    def getNextByValue(self, value):
        """
        Returns the node in the 'next' linked list with the given value
        :param value:
        char value
        :return:
        Node
        """
        traverse = self.getNext()
        if traverse == None:
            return None
        if traverse.getValue() == value:
            return traverse
        while traverse.hasAdjacent():
            if traverse.getValue() == value:
                return traverse
            traverse = traverse.getAdjacent()
        if traverse.getValue() == value:
            return traverse
        else:
            return None

    def addAdjacent(self, string):
        """
        Recursively add a string to the adjacent linked list
        :param string:
        String string
        :return:
        Node
        """
        traverse = self.getAdjacent()