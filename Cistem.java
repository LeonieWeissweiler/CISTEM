public class Cistem {;

	public static String stem(String word) {
		return stem(word, false);
	}
	
	public static String stem(String word, boolean case_insensitive) {
		if (word.length() == 0) return word;
		
		word = word.replace("Ü", "U");
		word = word.replace("Ö", "O");
		word = word.replace("Ä", "A");
		word = word.replace("ü", "u");
		word = word.replace("ö", "o");
		word = word.replace("ä", "a");
		
		boolean uppercase = Character.isUpperCase(word.charAt(0));
		
		word = word.toLowerCase();
		
		word = word.replace("ß", "ss");
		word = word.replaceAll("^ge(.{4,})", "$1");
		word = word.replace("sch", "$");
		word = word.replace("ei", "%");
		word = word.replace("ie", "&");
		
		word = word.replaceAll("(.)\\1", "$1*");
		
		while (word.length() > 3) {
			if (word.length() > 5) {
				String newWord = word.replaceAll("e[mr]$", "");
				if (!word.equals(newWord)) {
					word = newWord;
					continue;
				}
				
				newWord = word.replaceAll("nd$", "");
				if (!word.equals(newWord)) {
					word = newWord;
					continue;
				}
			}
			
			if (!uppercase || case_insensitive) {
				String newWord = word.replaceAll("t$", "");
				if (!word.equals(newWord)) {
					word = newWord;
					continue;
				}
			}
			
			String newWord = word.replaceAll("[esn]$", "");
			if (!word.equals(newWord)) {
				word = newWord;
				continue;
			} else {
				break;
			}
		}
		
		word = word.replaceAll("(.)\\*", "$1$1");
		word = word.replace("&", "ie");
		word = word.replace("%", "ei");
		word = word.replace("$", "sch");
		
		return word;
	}
	
	public static String[] segment(String word) {
		return segment(word, false);
	}
	
	public static String[] segment(String word, boolean case_insensitive) {
		if (word.length() == 0) {
			String[] result = new String[2];
			result[0] = "";
			result[1] = "";
			return result;
		} 
		
		int restLength = 0;
		boolean uppercase = Character.isUpperCase(word.charAt(0));
		word = word.toLowerCase();
		String original = new String(word);
		
		word = word.replace("sch", "$");
		word = word.replace("ei", "%");
		word = word.replace("ie", "&");
		
		word = word.replaceAll("(.)\\1", "$1*");
		
		while (word.length() > 3) {
			if (word.length() > 5) {
				String newWord = word.replaceAll("e[mr]$", "");
				if (!word.equals(newWord)) {
					restLength += 2;
					word = newWord;
					continue;
				}
				
				newWord = word.replaceAll("nd$", "");
				if (!word.equals(newWord)) {
					restLength += 2;
					word = newWord;
					continue;
				}
			}
			
			if (!uppercase || case_insensitive) {
				String newWord = word.replaceAll("t$", "");
				if (!word.equals(newWord)) {
					restLength += 1;
					word = newWord;
					continue;
				}
			}
			
			String newWord = word.replaceAll("[esn]$", "");
			if (!word.equals(newWord)) {
				restLength += 1;
				word = newWord;
				continue;
			} else {
				break;
			}
		}
		
		word = word.replaceAll("(.)\\*", "$1$1");
		word = word.replace("&", "ie");
		word = word.replace("%", "ei");
		word = word.replace("$", "sch");

		String rest = "";		
		if (restLength != 0) {
			rest = original.substring(original.length() - restLength);
		}
		
		String[] result = new String[2];
		result[0] = word;
		result[1] = rest;
		return result;
	}
}