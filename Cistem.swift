//
//  cistem.swift
//  cistem
//
//  Created by Hendrik Noeller on 15.09.17.
//  Copyright © 2017 de.HendrikNoeller. All rights reserved.
//

import Foundation
extension String {
	func index(_ index:Int) -> String.Index{
		if (index < 0) {
			return self.index(self.endIndex, offsetBy: index)
		} else {
			return self.index(self.startIndex, offsetBy: index)
		}
	}
}

struct Cistem {
	
	static let doubleCharacterRegex = try! NSRegularExpression(pattern: "(.)\\1")
	static let doubleCharacterReverseRegex = try! NSRegularExpression(pattern: "(.)\\*")
	
	private static func isUppercase(_ word: String) -> Bool {
		let first = String(word[word.startIndex])
		let firstUpper = first.uppercased()
		return first == firstUpper
	}
	
	static func stem(_ word: String, caseInsensitive: Bool = false) -> String {
		if (word.isEmpty) {
			return ""
		}
		let uppercase = isUppercase(word)
		
		var result = word
		result = result.lowercased()
		
		result = result.replacingOccurrences(of: "ü", with: "u")
		result = result.replacingOccurrences(of: "ö", with: "o")
		result = result.replacingOccurrences(of: "ä", with: "a")
		result = result.replacingOccurrences(of: "ß", with: "ss")
		
		if (result.characters.count >= 6 && result.hasPrefix("ge")) {
			result = result[result.index(2)..<result.endIndex]
		}
		
		result = result.replacingOccurrences(of: "sch", with: "$")
		result = result.replacingOccurrences(of: "ei", with: "%")
		result = result.replacingOccurrences(of: "ie", with: "&")
		
		
		result = doubleCharacterRegex.stringByReplacingMatches(in: result, options: [], range: NSMakeRange(0, result.characters.count), withTemplate: "$1*")
		
		while (result.characters.count > 3) {
			if (result.characters.count > 5) {
				if (result.hasSuffix("em") || result.hasSuffix("er") || result.hasSuffix("nd")){
					result = result[result.startIndex..<result.index(-2)]
					continue
				}
			}
			
			if (!uppercase || caseInsensitive) {
				if (result.hasSuffix("t")) {
					result = result[result.startIndex..<result.index(-1)]
					continue
				}
			}
			
			if (result.hasSuffix("e") || result.hasSuffix("s") || result.hasSuffix("n")) {
				result = result[result.startIndex..<result.index(-1)]
				continue
			}
			break
		}
		
		
		result = doubleCharacterReverseRegex.stringByReplacingMatches(in: result, options: [], range: NSMakeRange(0, result.characters.count), withTemplate: "$1$1")
		
		result = result.replacingOccurrences(of: "$", with: "sch")
		result = result.replacingOccurrences(of: "%", with: "ei")
		result = result.replacingOccurrences(of: "&", with: "ie")
		
		return result
	}
	
	
	static func segment(_ word: String, caseInsensitive: Bool = false) -> [String] {
		if (word.isEmpty) {
			return ["", ""]
		}
		let uppercase = isUppercase(word)
		var restLength = 0
		
		var result = word
		result = result.lowercased()
		let original = result
		
		result = result.replacingOccurrences(of: "sch", with: "$")
		result = result.replacingOccurrences(of: "ei", with: "%")
		result = result.replacingOccurrences(of: "ie", with: "&")
		
		
		result = doubleCharacterRegex.stringByReplacingMatches(in: result, options: [], range: NSMakeRange(0, result.characters.count), withTemplate: "$1*")
		
		while (result.characters.count > 3) {
			if (result.characters.count > 5) {
				if (result.hasSuffix("em") || result.hasSuffix("er") || result.hasSuffix("nd")){
					restLength += 2
					result = result[result.startIndex..<result.index(-2)]
					continue
				}
			}
			
			if (!uppercase || caseInsensitive) {
				if (result.hasSuffix("t")) {
					restLength += 1
					result = result[result.startIndex..<result.index(-1)]
					continue
				}
			}
			
			if (result.hasSuffix("e") || result.hasSuffix("s") || result.hasSuffix("n")) {
				restLength += 1
				result = result[result.startIndex..<result.index(-1)]
				continue
			}
			break
		}
		
		result = doubleCharacterReverseRegex.stringByReplacingMatches(in: result, options: [], range: NSMakeRange(0, result.characters.count), withTemplate: "$1$1")
		
		result = result.replacingOccurrences(of: "$", with: "sch")
		result = result.replacingOccurrences(of: "%", with: "ei")
		result = result.replacingOccurrences(of: "&", with: "ie")
		
		var rest = ""
		if (restLength > 0) {
			rest = String(original[original.index(-restLength)..<original.endIndex])
		}
		
		return [result, rest]
	}
	
}
