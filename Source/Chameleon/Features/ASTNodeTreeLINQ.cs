using System.Linq;
using System.Collections.Generic;
using System;
 

namespace Chameleon.Parsing
{
	/// <summary>
    /// Defines an interface that must be implemented to generate the LinqToTree methods
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public interface ILinqTree<T>
    {
        IEnumerable<T> Children();

        T Parent { get; }
    }
  
    public static class TreeExtensions
    {
        /// <summary>
        /// Returns a collection of descendant elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Descendants(this Chameleon.Parsing.ASTNode item)
        {
            ILinqTree<Chameleon.Parsing.ASTNode> adapter = new Chameleon.Parsing.ASTNodeTreeAdapter(item);
            foreach (var child in adapter.Children())
            {
                yield return child;

                foreach (var grandChild in child.Descendants())
                {
                    yield return grandChild;
                }
            }
        }    
           
        /// <summary>
        /// Returns a collection containing this element and all descendant elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> DescendantsAndSelf(this Chameleon.Parsing.ASTNode item)
        {            
            yield return item;

            foreach (var child in item.Descendants())
            {
                yield return child;
            }
        }
        
        /// <summary>
        /// Returns a collection of ancestor elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Ancestors(this Chameleon.Parsing.ASTNode item)
        {
            ILinqTree<Chameleon.Parsing.ASTNode> adapter = new Chameleon.Parsing.ASTNodeTreeAdapter(item);
            
            var parent = adapter.Parent;
            while(parent != null)
            {
                yield return parent;
                adapter = new Chameleon.Parsing.ASTNodeTreeAdapter(parent);
                parent = adapter.Parent;
            }
        } 
        
        /// <summary>
        /// Returns a collection containing this element and all ancestor elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> AncestorsAndSelf(this Chameleon.Parsing.ASTNode item)
        {
            yield return item;

            foreach (var ancestor in item.Ancestors())
            {
                yield return ancestor;
            }
        }
        
        /// <summary>
        /// Returns a collection of child elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Elements(this Chameleon.Parsing.ASTNode item)
        {
            ILinqTree<Chameleon.Parsing.ASTNode> adapter = new Chameleon.Parsing.ASTNodeTreeAdapter(item);
            foreach (var child in adapter.Children())
            {
                yield return child;                
            }
        }
        
        /// <summary>
        /// Returns a collection of the sibling elements before this node, in document order.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> ElementsBeforeSelf(this Chameleon.Parsing.ASTNode item)
        {
			if (item.Ancestors().FirstOrDefault()==null)
				yield break;
            foreach (var child in item.Ancestors().First().Elements())
            {
				if (child.Equals(item))
					break;
                yield return child;                
            }
        }
        
        /// <summary>
        /// Returns a collection of the elements after this node, in document order.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> ElementsAfterSelf(this Chameleon.Parsing.ASTNode item)
        {
			if (item.Ancestors().FirstOrDefault()==null)
				yield break;
            bool afterSelf = false;
            foreach (var child in item.Ancestors().First().Elements())
            {
				if (afterSelf)
					yield return child;                
                
                if (child.Equals(item))
					afterSelf=true;
            }
        }
        
        /// <summary>
        /// Returns a collection containing this element and all child elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> ElementsAndSelf(this Chameleon.Parsing.ASTNode item)
        {
            yield return item;

            foreach (var child in item.Elements())
            {
                yield return child;
            }
        }
      
        /// <summary>
        /// Returns a collection of descendant elements which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Descendants<T>(this Chameleon.Parsing.ASTNode item)
        {
            return item.Descendants().Where(i => i is T).Cast<Chameleon.Parsing.ASTNode>();
        }
        


		/// <summary>
        /// Returns a collection of the sibling elements before this node, in document order
        /// which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> ElementsBeforeSelf<T>(this Chameleon.Parsing.ASTNode item)
        {
			return item.ElementsBeforeSelf().Where(i => i is T).Cast<Chameleon.Parsing.ASTNode>();
        }
        
        /// <summary>
        /// Returns a collection of the after elements after this node, in document order
        /// which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> ElementsAfterSelf<T>(this Chameleon.Parsing.ASTNode item)
        {
			return item.ElementsAfterSelf().Where(i => i is T).Cast<Chameleon.Parsing.ASTNode>();
        }

        /// <summary>
        /// Returns a collection containing this element and all descendant elements
        /// which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> DescendantsAndSelf<T>(this Chameleon.Parsing.ASTNode item)
        {
            return item.DescendantsAndSelf().Where(i => i is T).Cast<Chameleon.Parsing.ASTNode>();
        }
        
        /// <summary>
        /// Returns a collection of ancestor elements which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Ancestors<T>(this Chameleon.Parsing.ASTNode item)
        {
            return item.Ancestors().Where(i => i is T).Cast<Chameleon.Parsing.ASTNode>();
        }
        
        /// <summary>
        /// Returns a collection containing this element and all ancestor elements
        /// which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> AncestorsAndSelf<T>(this Chameleon.Parsing.ASTNode item)
        {
            return item.AncestorsAndSelf().Where(i => i is T).Cast<Chameleon.Parsing.ASTNode>();
        }
        
        /// <summary>
        /// Returns a collection of child elements which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Elements<T>(this Chameleon.Parsing.ASTNode item)
        {
            return item.Elements().Where(i => i is T).Cast<Chameleon.Parsing.ASTNode>();
        }
        
        /// <summary>
        /// Returns a collection containing this element and all child elements.
        /// which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> ElementsAndSelf<T>(this Chameleon.Parsing.ASTNode item)
        {
            return item.ElementsAndSelf().Where(i => i is T).Cast<Chameleon.Parsing.ASTNode>();
        }
        
    }
    
    public static class EnumerableTreeExtensions
    {
		/// <summary>
        /// Applies the given function to each of the items in the supplied
        /// IEnumerable.
        /// </summary>
        private static IEnumerable<Chameleon.Parsing.ASTNode> DrillDown(this IEnumerable<Chameleon.Parsing.ASTNode> items,
            Func<Chameleon.Parsing.ASTNode, IEnumerable<Chameleon.Parsing.ASTNode>> function)
        {
            foreach(var item in items)
            {
                foreach(var itemChild in function(item))
                {
                    yield return itemChild;
                }
            }
        }

       
        /// <summary>
        /// Applies the given function to each of the items in the supplied
        /// IEnumerable, which match the given type.
        /// </summary>
        public static IEnumerable<Chameleon.Parsing.ASTNode> DrillDown<T>(this IEnumerable<Chameleon.Parsing.ASTNode> items,
            Func<Chameleon.Parsing.ASTNode, IEnumerable<Chameleon.Parsing.ASTNode>> function)
            where T : Chameleon.Parsing.ASTNode
        {
            foreach(var item in items)
            {
                foreach(var itemChild in function(item))
                {
                    if (itemChild is T)
                    {
                        yield return (T)itemChild;
                    }
                }
            }
        }

    
        /// <summary>
        /// Returns a collection of descendant elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Descendants(this IEnumerable<Chameleon.Parsing.ASTNode> items)
        {
            return items.DrillDown(i => i.Descendants());
        }    
           
        /// <summary>
        /// Returns a collection containing this element and all descendant elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> DescendantsAndSelf(this IEnumerable<Chameleon.Parsing.ASTNode> items)
        {            
            return items.DrillDown(i => i.DescendantsAndSelf());
        }
        
        /// <summary>
        /// Returns a collection of ancestor elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Ancestors(this IEnumerable<Chameleon.Parsing.ASTNode> items)
        {
            return items.DrillDown(i => i.Ancestors());
        } 
        
        /// <summary>
        /// Returns a collection containing this element and all ancestor elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> AncestorsAndSelf(this IEnumerable<Chameleon.Parsing.ASTNode> items)
        {
            return items.DrillDown(i => i.AncestorsAndSelf());
        }
        
        /// <summary>
        /// Returns a collection of child elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Elements(this IEnumerable<Chameleon.Parsing.ASTNode> items)
        {
            return items.DrillDown(i => i.Elements());
        }
        
        /// <summary>
        /// Returns a collection containing this element and all child elements.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> ElementsAndSelf(this IEnumerable<Chameleon.Parsing.ASTNode> items)
        {
            return items.DrillDown(i => i.ElementsAndSelf());
        }

       
        /// <summary>
        /// Returns a collection of descendant elements which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Descendants<T>(this IEnumerable<Chameleon.Parsing.ASTNode> items)
	        where T : Chameleon.Parsing.ASTNode
        {
            return items.DrillDown<T>(i => i.Descendants());
        }
        
        /// <summary>
        /// Returns a collection containing this element and all descendant elements.
        /// which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> DescendantsAndSelf<T>(this IEnumerable<Chameleon.Parsing.ASTNode> items)
	        where T : Chameleon.Parsing.ASTNode
        {
            return items.DrillDown<T>(i => i.DescendantsAndSelf());
        }
        
        /// <summary>
        /// Returns a collection of ancestor elements which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Ancestors<T>(this IEnumerable<Chameleon.Parsing.ASTNode> items)
	        where T : Chameleon.Parsing.ASTNode
        {
            return items.DrillDown<T>(i => i.Ancestors());
        }
        
        /// <summary>
        /// Returns a collection containing this element and all ancestor elements.
        /// which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> AncestorsAndSelf<T>(this IEnumerable<Chameleon.Parsing.ASTNode> items)
	        where T : Chameleon.Parsing.ASTNode
        {
            return items.DrillDown<T>(i => i.AncestorsAndSelf());
        }
        
        /// <summary>
        /// Returns a collection of child elements which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> Elements<T>(this IEnumerable<Chameleon.Parsing.ASTNode> items)
	        where T : Chameleon.Parsing.ASTNode
        {
            return items.DrillDown<T>(i => i.Elements());
        }
        
        /// <summary>
        /// Returns a collection containing this element and all child elements.
        /// which match the given type.
        /// </summary>
	    public static IEnumerable<Chameleon.Parsing.ASTNode> ElementsAndSelf<T>(this IEnumerable<Chameleon.Parsing.ASTNode> items)
	        where T : Chameleon.Parsing.ASTNode
        {
            return items.DrillDown<T>(i => i.ElementsAndSelf());
        }
    }
}