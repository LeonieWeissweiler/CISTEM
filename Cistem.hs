{-# LANGUAGE OverloadedStrings, RecordWildCards #-}

module NLP.Stemmer.Cistem (stem,stemCaseInsensitive,Segmentation(..),segment',segment,segment'CaseInsensitive,segmentCaseInsensitive) where

import Data.Char
import Data.Monoid
import Data.Text as T

-- | Guess the word stem. This module uses the CISTEM algorithm, published by L. Weißweiler and A. Fraser in "Developing a Stemmer for German Based on a Comparative Analysis of Publicly Available Stemmers" (2017).
stem :: Text -> Text
stem t =
    let firstUpper = isUpper (T.head t)
    in postpare $ loop firstUpper $ prepare t

-- | A case insensitive variant. Use only if the text may be incorrectly upper case.
stemCaseInsensitive :: Text -> Text
stemCaseInsensitive t = postpare $ loop False $ prepare t

data Segmentation = Segmentation { segPrefix :: Text, segStem :: Text, segSuffix :: Text } deriving (Show,Eq)
                        
-- | Split the word into a prefix, the stem and a suffix. In contrast to the `stem` function umlauts remain unchanged.
segment' :: Text -> Segmentation
segment' t =
    let firstUpper = isUpper (T.head t)
        lower = T.toLower t
        prepared = segmentPrepare t
        theStem = postpare $ loop firstUpper prepared
        thePrefix | theStem `isPrefixOf` lower = ""
                  | "ge" `isPrefixOf` lower = "ge"
                  | otherwise = error ("segment' should be debugged; extracted stem: "++ unpack theStem)
        theSuffix = T.drop (T.length thePrefix + T.length theStem) lower
    in Segmentation thePrefix theStem theSuffix

-- | Split the word into stem and suffix. This is supposed to be compatible to the `segment` function from the reference implementation.
segment :: Text -> (Text,Text)
segment t =
    let Segmentation{..} = segment' t
    in (segPrefix<>segStem, segSuffix)

-- | A case insensitive variant. Use only if the text may be incorrectly upper case.
segmentCaseInsensitive :: Text -> (Text,Text)
segmentCaseInsensitive = segment . T.toLower

-- | A case insensitive variant. Use only if the text may be incorrectly upper case.
segment'CaseInsensitive :: Text -> Segmentation
segment'CaseInsensitive = segment' . T.toLower
                         
loop u t | T.length t <= 3 = t
         | (T.length t > 5) && (["em","er","nd"] `isSuffixOf'` t) = loop u (stripSuffix' ["em","er","nd"] t)
         | not u && ("t" `isSuffixOf` t) = loop u (stripSuffix' ["t"] t)
         | ["e","s","n"] `isSuffixOf'` t = loop u (stripSuffix' ["e","s","n"] t)
         | otherwise = t
                                           
       
prepare :: Text -> Text
prepare =
    replace "ü" "u" .
    replace "ö" "o" .
    replace "ä" "a" .
    replxx .
    replace "ß" "ss" .
    segmentPrepare

segmentPrepare :: Text -> Text
segmentPrepare =
    replace "sch" "$" .
    replace "ie" "&" .
    replace "ei" "%" .
    replxx .
    stripge .
    T.toLower
     
postpare :: Text -> Text
postpare =
    replace "%" "ei" .
    replace "&" "ie" .
    replace "$" "sch" .
    replxxback
     
replxx :: Text -> Text
replxx = snd . mapAccumL f '\0'
    where f prev curr | prev == curr = (curr,'*')
                      | otherwise = (curr,curr)

replxxback :: Text -> Text
replxxback = snd . mapAccumL f '\0'
    where f prev '*' = (prev,prev)
          f prev curr = (curr,curr)

stripge :: Text -> Text
stripge t | T.length t >= 6 =
              case stripPrefix "ge" t of
                Nothing -> t
                Just t -> t
          | otherwise = t

isSuffixOf' [] _ = False
isSuffixOf' (s:ss) t = (s `isSuffixOf` t) || (ss `isSuffixOf'` t)

stripSuffix' :: [Text] -> Text -> Text
stripSuffix' [] hay = hay
stripSuffix' (suff:ss) hay =
    case stripSuffix suff hay of
      Just t -> t
      Nothing -> stripSuffix' ss hay
