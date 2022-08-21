#zk_slist_middle
Get the middle of a linked list. Use the 2 pointers technic

#zk_slist_has_cycle
Solution 3: Floyd’s Cycle-Finding Algorithm
Approach: This is the fastest method and has been described below:

Traverse linked list using two pointers.
Move one pointer(slow_p) by one and another pointer(fast_p) by two.
If these pointers meet at the same node then there is a loop. If pointers do not meet then linked list doesn’t have a loop.


#zk_slist_has_intersection.
The time complexity of this method is O(m+n) and the used Auxiliary space is O(1)
Method 6 (Traverse both lists and compare addresses of last nodes) This method is only to detect if there is an intersection point or not. (Thanks to NeoTheSaviour for suggesting this)
1) Traverse the list 1, store the last node address
2) Traverse the list 2, store the last node address.
3) If nodes stored in 1 and 2 are same then they are intersecting.

#zk_slist_intersection
Method 3(Using the difference in node counts)
Time Complexity: O(m+n)
Auxiliary Space: O(1)
Get the count of the nodes in the first list, let the count be c1.
Get the count of the nodes in the second list, let the count be c2.
Get the difference of counts d = abs(c1 – c2)
Now traverse the bigger list from the first node to d nodes so that from here onwards both the lists have an equal no of nodes
Then we can traverse both lists in parallel till we come across a common node. (Note that getting a common node is done by comparing the address of the nodes)

Method 8( 2-pointer technique ):

Using Two pointers :

Initialize two pointers ptr1 and ptr2  at head1 and  head2.
Traverse through the lists, one node at a time.
When ptr1 reaches the end of a list, then redirect it to head2.
similarly, when ptr2 reaches the end of a list, redirect it to the head1.
Once both of them go through reassigning, they will be equidistant from
 the collision point
If at any node ptr1 meets ptr2, then it is the intersection node.
After the second iteration if there is no intersection node it returns NULL.
