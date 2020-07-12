package chen;

import java.util.Map;
import java.util.TreeMap;

public class cmap {

	
	
	
	
	static TreeMap<String, String> m_map = new TreeMap<String, String>();
	
	
	public static boolean init()
	{
		//78, 23, 56, 12, 1, 100, 23, 11, 9, 2
		cmap.put("!", "1");
		cmap.put("#", "35");
		cmap.put("&", "2");
		cmap.put("A","65");
		cmap.put("1", "49");
		cmap.put("6", "9");
		cmap.put("B","11");
		cmap.put("C","67");
		cmap.put("D","68");
		cmap.put("E","69");
		cmap.put("F","70");
		cmap.put("I","12");
		cmap.put("M","23");
		cmap.put("Q","23");
		cmap.put("W","56");
		cmap.put("P","80");
		cmap.put("X","88");
		cmap.put("a", "78");
		
		cmap.put("z", "100");
		
		return true;
	}
	
	public static String get_value(String key)
	{
		
		return m_map.get(key);
	}
	public static boolean is_key(String key)
	{
		return m_map.containsKey(key);
	}
	public static boolean put(String key, String value)
	{
		 String old_value = m_map.put(key, value);
		
		 if(old_value == null)
		 {
			 System.out.println("install ok map key =" + key + ", value = " +value);
			 return true;
		 }
		 System.out.println("install failed map key =" + key + ", value = " +value + ", old_value = " +old_value);
		 return true;
		
	}
	
	
}
