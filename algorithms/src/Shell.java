import src.cmap;

/******************************************************************************
 *  Compilation:  javac Shell.java
 *  Execution:    java Shell < input.txt
 *  Dependencies: StdOut.java StdIn.java
 *  Data files:   https://algs4.cs.princeton.edu/21elementary/tiny.txt
 *                https://algs4.cs.princeton.edu/21elementary/words3.txt
 *   
 *  Sorts a sequence of strings from standard input using shellsort.
 *
 *  % more tiny.txt
 *  S O R T E X A M P L E
 *
 *  % java Shell < tiny.txt
 *  A E E L M O P R S T X                 [ one string per line ]
 *    
 *  % more words3.txt
 *  bed bug dad yes zoo ... all bad yet
 *  
 *  % java Shell < words3.txt
 *  all bad bed bug dad ... yes yet zoo    [ one string per line ]
 *
 *
 ******************************************************************************/



/**
 *  The {@code Shell} class provides static methods for sorting an
 *  array using <em>Shellsort</em> with
 *  <a href = "https://oeis.org/A003462"> Knuth's increment sequence</a>
 *  (1, 4, 13, 40, ...). In the worst case, this implementation makes
 *  &Theta;(<em>n</em><sup>3/2</sup>) compares and exchanges to sort
 *  an array of length <em>n</em>.
 *  <p>
 *  This sorting algorithm is not stable.
 *  It uses &Theta;(1) extra memory (not including the input array).
 *  <p>
 *  For additional documentation, see
 *  <a href="https://algs4.cs.princeton.edu/21elementary">Section 2.1</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *  
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
public class Shell {

    // This class should not be instantiated.
    private Shell() { }

    /**
     * 当前数组中的元素
     * @param a
     * @param h
     * @param parent
     */
    public static void show(Comparable[] a, int h, int parent, int array_len)
    {
    	int  n = a.length;
    	try {
 			Thread.currentThread().sleep(1000) ;
 		} catch (InterruptedException e) {
 			// TODO Auto-generated catch block
 			e.printStackTrace();
 		}
        StdDraw.clear();
        for (int num = 0; num <  n; ++num)
        {
        	 //乘以1.0为的是将其变为double类型
            double x = 1.0 * num /n +0.05;
            
            double y = Integer.valueOf( cmap.get_value( (String) a[num])) / 200.0;
 
            double width = 0.5 / (n +1)  ;
 
            double high = Integer.valueOf( cmap.get_value( (String) a[num]))  / 200.0;
            
        	if (parent == num) //记录当前数组中最小的下标
            {
             	StdDraw.setPenColor(StdDraw.BLUE);
            } 
        	else if (((num ) % h == (array_len % h) && num <= array_len) || array_len == num) //是当前要比较数组中元素都显示绿色
            {
             	StdDraw.setPenColor(StdDraw.GREEN);
             }
            else 
            { //不是当前数组的元素显示颜色
            	 
//                 else
                 {
                 	  StdDraw.setPenColor(StdDraw.MAGENTA);
                 }
            }
            StdDraw.filledRectangle(x, y, width, high);
            StdDraw.setPenColor(StdDraw.YELLOW);
            System.out.println("x =" + x);
            if (((num ) % h == (array_len % h) && num <= array_len) || array_len == num)
            {
            	 StdDraw.text(x, y, "h="+ h +"- "+ cmap.get_value( (String) a[num])); 	
            }
            else 
            {
            	 StdDraw.text(x, y, cmap.get_value( (String) a[num]));
            }
           
        }
        StdDraw.show();
    }
    /**
     * 两个比较的元素
     * @param a
     * @param h
     * @param parent
     * @param cur
     * @param array_len 
     */
    public static void show(Comparable[] a, int h, int parent, int cur, int array_len)
    {
    	 StdOut.println(" red h = " + h + ", parent = " + parent + ", cur = " + cur + ", array_len = " + array_len);
    	
    	
    	int  n = a.length;
    	try {
 			Thread.currentThread().sleep(1000) ;
 		} catch (InterruptedException e) {
 			// TODO Auto-generated catch block
 			e.printStackTrace();
 		}
        StdDraw.clear();
        for (int num = 0; num <  n; ++num)
        {
        	 //乘以1.0为的是将其变为double类型
            double x = 1.0 * num /n +0.05;
            
            double y = Integer.valueOf( cmap.get_value( (String) a[num])) / 200.0;
 
            double width = 0.5 / (n +1)  ;
 
            double high = Integer.valueOf( cmap.get_value( (String) a[num]))  / 200.0;
            //当前节点数据显示红色
        	if (cur == num)
            {
             	StdDraw.setPenColor(StdDraw.RED);
            } 
        	else if (parent == num) //记录当前数组中最小的下标
            {
             	StdDraw.setPenColor(StdDraw.BLUE);
            } 
        	else if (((num ) % h == (array_len % h) && num <= array_len) || array_len == num) //是当前要比较数组中元素都显示绿色
            {
             	StdDraw.setPenColor(StdDraw.GREEN);
            }
            else 
            { //不是当前数组的元素显示颜色
            	 
//                 else
                 {
                 	  StdDraw.setPenColor(StdDraw.MAGENTA);
                 }
            }
            StdDraw.filledRectangle(x, y, width, high);
            StdDraw.setPenColor(StdDraw.YELLOW);
            System.out.println("x =" + x);
            if (((num ) % h == (array_len % h) && num <= array_len) || array_len == num)
            {
            	 StdDraw.text(x, y, "h="+ h +"-"+ cmap.get_value( (String) a[num])); 	
            }
            else 
            {
            	 StdDraw.text(x, y, cmap.get_value( (String) a[num]));
            }
        }
        StdDraw.show();
    }
    /**
     * Rearranges the array in ascending order, using the natural order.
     * @param a the array to be sorted
     */
    public static void sort(Comparable[] a) {
        int n = a.length;

        // 3x+1 increment sequence:  1, 4, 13, 40, 121, 364, 1093, ... 
        int h = 1;
        while (h < n/3) 
        {
        	h = 3*h + 1;
        }

        while (h >= 1)
        {
//        	show(a, h, -1, -1);
            // h-sort the array
            for (int i = h; i < n; i++) 
            {
            	show(a, h, i, i);
                for (int j = i; j >= h ; j -= h) 
                {
                	show(a, h, j, j-h, i);
                	if (less(a[j], a[j-h]))
                	{
                		exch(a, j, j-h);
                        show(a);
                	}
                	else 
                	{
                		break;
                	}
                    
                    show(a, h, j, i);
                }
            }
            assert isHsorted(a, h); 
            h /= 3;
        }
        assert isSorted(a);
        StdDraw.show(a, a.length);
    }



   /***************************************************************************
    *  Helper sorting functions.
    ***************************************************************************/
    
    // is v < w ?
    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }
        
    // exchange a[i] and a[j]
    private static void exch(Object[] a, int i, int j) {
        Object swap = a[i];
        a[i] = a[j];
        a[j] = swap;
    }


   /***************************************************************************
    *  Check if array is sorted - useful for debugging.
    ***************************************************************************/
    private static boolean isSorted(Comparable[] a) {
        for (int i = 1; i < a.length; i++)
            if (less(a[i], a[i-1])) return false;
        return true;
    }

    // is the array h-sorted?
    private static boolean isHsorted(Comparable[] a, int h) {
        for (int i = h; i < a.length; i++)
            if (less(a[i], a[i-h])) return false;
        return true;
    }

    // print array to standard output
    private static void show(Comparable[] a) {
        for (int i = 0; i < a.length; i++)
        {
            StdOut.print(cmap.get_value((String)a[i]) + "  ");
        }
        StdOut.println();
    }

    /**
     * Reads in a sequence of strings from standard input; Shellsorts them; 
     * and prints them to standard output in ascending order. 
     *
     * @param args the command-line arguments
     */
    public static void main(String[] args) {
//        String[] a = StdIn.readAllStrings();
//    	String[] a = new String[10] ;
    	 String[] a = new String[10] ;
    	 a[0] = "a";
       	 a[1] = "M";
       	 a[2] = "W";
       	 a[3] = "I";
       	 a[4] = "!";
       	 a[5] = "z";
       	 a[6] = "Q";
       	 a[7] = "B";
       	 a[8] = "6";
       	 a[9] = "&";
       	 cmap.init();
        Shell.sort(a);
        show(a);
    }

}

/******************************************************************************
 *  Copyright 2002-2020, Robert Sedgewick and Kevin Wayne.
 *
 *  This file is part of algs4.jar, which accompanies the textbook
 *
 *      Algorithms, 4th edition by Robert Sedgewick and Kevin Wayne,
 *      Addison-Wesley Professional, 2011, ISBN 0-321-57351-X.
 *      http://algs4.cs.princeton.edu
 *
 *
 *  algs4.jar is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  algs4.jar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with algs4.jar.  If not, see http://www.gnu.org/licenses.
 ******************************************************************************/
