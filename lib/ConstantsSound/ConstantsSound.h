#pragma once

#define ARM_MATH_CM0PLUS
#include <arm_math.h>

///// Circuit:
/*
 * Arduino/Genuino Zero, MKRZero or MKR1000 board
 * ICS43432:
   * GND connected GND
   * 3.3V connected 3.3V (Zero) or VCC (MKR1000, MKRZero)
   * WS connected to pin 0 (Zero) or pin 3 (MKR1000, MKRZero)
   * CLK connected to pin 1 (Zero) or pin 2 (MKR1000, MKRZero)
   * SD connected to pin 9 (Zero) or pin A6 (MKR1000, MKRZero)
*/

///// MICROPHONE PROPERTIES (INVENSENSE)

const int BIT_LENGTH = 24;
const int FULL_SCALE_DBSPL = 120; // FULL SCALE dBSPL (AOP = 116dB SPL)
const double FULL_SCALE_DBFS = 20*log10(pow(2,(BIT_LENGTH)));
const double RMS_HANN = 0.61177;
const int CONST_FACTOR = 64;

///// WEIGHTING TABLES

//Table for 44100Hz and 256 fftSize

  #define WEIGHTINGTA_44100Hz_256 {0,0.0862538915555343,0.239252440067295,0.381499718566378,0.506174881840981,0.613491046469462,0.704657091342527,0.781358835632495,0.845492532387227,0.898943803532723,0.943446758613464,0.980518052821849,1.011442168756,1.03728522618051,1.05892148026641,1.07706317893995,1.09228900743996,1.10506909425038,1.11578603774419,1.12475213161783,1.13222326008119,1.13841000815891,1.14348650518794,1.14759745249792,1.15086370996276,1.15338674443019,1.15525218115916,1.15653264835139,1.15729006385055,1.15757748064798,1.15744058242197,1.15691890053426,1.15604680851176,1.15485433808098,1.15336785152296,1.15161059787385,1.14960317484241,1.14736391388858,1.144909202431,1.14225375441076,1.13941083827188,1.13639246969807,1.13320957507457,1.12987213054712,1.12638928067012,1.12276943992611,1.1190203798254,1.11514930382876,1.11116291195666,1.10706745663856,1.10286879110129,1.09857241138616,1.09418349291168,1.08970692235556,1.08514732551056,1.08050909166977,1.07579639501394,1.0710132134039,1.06616334492279,1.0612504224636,1.05627792661571,1.05124919706901,1.04616744272406,1.04103575067143,1.03585709418121,1.03063433982534,1.02537025383928,1.02006750781565,1.0147286838109,1.00935627893557,1.00395270948995,0.998520314699319,0.99306136009617,0.987578040591138,0.982072483269213,0.976546749943447,0.971002839494508,0.965442690021033,0.959868180822797,0.954281134236092,0.948683317338438,0.943076443537719,0.937462174059082,0.931842119341346,0.92621784035331,0.920590849839106,0.914962613500672,0.909334551124467,0.903708037658683,0.898084404246482,0.892464939220101,0.886850889060089,0.881243459323409,0.875643815543675,0.870053084106385,0.864472353101651,0.858902673156583,0.853345058249221,0.847800486505647,0.842269900981679,0.836754210430351,0.83125429005623,0.825770982257424,0.820305097356057,0.814857414317805,0.809428681461038,0.804019617155993,0.79863091051432,0.793263222069304,0.787917184446967,0.782593403028247,0.777292456602357,0.772014898011446,0.766761254786601,0.761532029775251,0.756327701759963,0.751148726068651,0.745995535176175,0.740868539297297,0.735768126970984,0.730694665635993,0.725648502197718,0.720629963586252,0.715639357305606,0.710676971974081,0.705743077855713,0.700837927382786,0.695961755669379,0.691114781015906,0.686297205404659,0.681509214986311,0.676750980557397,0.672022658028753,0.667324388884942,0.662656300634656,0.658018507252134,0.653411109609613,0.648834195900849,0.644287842055744,0.639772112146134,0.635287058782778,0.63083272350362,0.626409137153377,0.622016320254532,0.617654283369795,0.613323027456127,0.609022544210399,0.60475281640678,0.600513818225947,0.596305515576211,0.592127866406669,0.587980821012465,0.583864322332297,0.579778306238247,0.575722701818072,0.571697431650051,0.567702412070528,0.563737553434233,0.559802760367541,0.555897932014755,0.552022962277563,0.54817774004777,0.544362149433441,0.540576069978573,0.536819376876427,0.533091941176635,0.529393629986214,0.52572430666461,0.522083831012889,0.518472059457211,0.5148888452267,0.511334038525829,0.507807486701459,0.504309034404633,0.500838523747253,0.497395794453758,0.493980684007924,0.490593027794891,0.487232659238545,0.483899409934353,0.480593109777784,0.477313587088399,0.474060668729743,0.470834180225132,0.467633945869448,0.464459788837038,0.461311531285828,0.45818899445774,0.455091998775533,0.45202036393613,0.448973909000568,0.445952452480626,0.442955812422246,0.439983806485832,0.437036252023507,0.434112966153428,0.431213765831224,0.428338467918663,0.42548688924961,0.422658846693364,0.419854157215444,0.417072637935918,0.41431410618532,0.411578379558249,0.408865275964721,0.406174613679325,0.403506211388266,0.400859888234358,0.398235463860026,0.395632758448378,0.39305159276242,0.390491788182462,0.387953166741769,0.385435551160528,0.382938764878175,0.380462632084138,0.378006977747047,0.375571627642463,0.373156408379176,0.370761147424114,0.368385673125924,0.366029814737245,0.363693402435748,0.36137626734396,0.359078241547929,0.356799158114758,0.354538851109067,0.352297155608395,0.350073907717599,0.347868944582282,0.345682104401278,0.343513226438233,0.341362151032316,0.339228719608089,0.337112774684568,0.335014159883506,0.332932719936925,0.330868300693929,0.32882074912682,0.326789913336546,0.324775642557513,0.322777787161772,0.32079619866262,0.318830729717626,0.316881234131118,0.314947566856136,0.313029583995888,0.31112714280472,0.309240101688628,0.307368320205317,0.305511659063845,0.303669980123854,0.301843146394414,0.30003102203249,0.298233472341061,0.29645036376689,0.294681563897973}

//To be applied in the non-DB spectrum
const double WEIGHTINGTAB[] = WEIGHTINGTA_44100Hz_256;

///// HANN WINDOW

  #define HANN_WINDOW_512 {0,0.0000377965772740962,0.000151180594771427,0.000340134910380874,0.000604630956796859,0.000944628745838338,0.00136007687449447,0.00185091253269609,0.00241706151281168,0.00305843822086654,0.00377494568948339,0.00456647559254264,0.00543290826155973,0.0063741127037773,0.00738994662196968,0.00848025643595607,0.00964487730581998,0.0108836331568305,0.0121963367060627,0.0135827894907121,0.0150427818980994,0.0165760931973613,0.0181824915728215,0.019861734159039,0.0216135670775249,0.0234377254751261,0.0253339335640674,0.0273019046636466,0.0293413412435765,0.0314519349689681,0.033633366746946,0.0358853067748912,0.0382074145903025,0.0405993391222701,0.0430607187445522,0.0455911813302485,0.0481903443080604,0.0508578147201305,0.0535931892814526,0.0563960544408427,0.0592659864434626,0.0622025513948853,0.0652053053266945,0.0682737942636061,0.0714075542921032,0.0746061116305735,0.0778689827009385,0.0811956742017641,0.0845856831828408,0.0880384971212229,0.091553593998715,0.0951304423807943,0.0987685014969555,0.102467221322468,0.106226042661535,0.110044397231829,0.113921707750418,0.117857388021034,0.121850843022703,0.125901468999704,0.130008653552846,0.134171775732054,0.138390206130252,0.14266330697852,0.146990432242509,0.151370927720124,0.15580413114042,0.160289372263735,0.164825972983019,0.169413247426352,0.174050502060644,0.17873703579648,0.183472140094124,0.188255099070633,0.193085189608094,0.197961681462944,0.20288383737638,0.207850913185816,0.212862157937393,0.217916813999513,0.223014117177383,0.228153296828549,0.233333575979408,0.238554171442674,0.243814293935788,0.249113148200246,0.254449933121827,0.259823841851719,0.265234061928494,0.270679775400948,0.276160158951759,0.281674384021968,0.287221616936238,0.292801019028898,0.29841174677074,0.304052951896545,0.309723781533331,0.315423378329296,0.321150880583437,0.326905422375828,0.332686133698534,0.338492140587147,0.344322565252915,0.350176526215451,0.356053138436005,0.361951513451266,0.367870759507683,0.373809981696295,0.379768282088018,0.385744759869409,0.391738511478851,0.397748630743159,0.403774209014585,0.40981433530819,0.415868096439573,0.421934577162933,0.42801286030944,0.434102026925899,0.440201156413684,0.446309326667918,0.452425614216887,0.45854909436165,0.464678841315845,0.470813928345655,0.476953427909915,0.483096411800347,0.489241951281889,0.49538911723311,0.501536980286678,0.507684610969869,0.513831079845091,0.5199754576504,0.526116815439995,0.532254224724658,0.538386757612132,0.544513486947405,0.550633486452881,0.556745830868423,0.56284959609124,0.568943859315595,0.575027699172326,0.581100195868139,0.587160431324672,0.593207489317293,0.599240455613625,0.605258418111759,0.611260466978157,0.617245694785205,0.623213196648401,0.629162070363163,0.635091416541232,0.641000338746643,0.646887943631258,0.652753341069825,0.658595644294553,0.664413970029181,0.670207438622517,0.675975174181427,0.68171630470326,0.687429962207682,0.693115282867903,0.698771407141278,0.704397479899253,0.709992650556653,0.715556073200279,0.721086906716794,0.726584314919893,0.73204746667672,0.737475536033525,0.742867702340536,0.748223150376032,0.75354107046959,0.7588206586245,0.764061116639314,0.769261652228528,0.774421479142359,0.779539817285623,0.784615892835666,0.789648938359361,0.794638192929131,0.799582902237994,0.804482318713598,0.809335701631252,0.814142317225904,0.818901438803083,0.823612346848765,0.828274329138148,0.832886680843338,0.837448704639903,0.841959710812305,0.84641901735817,0.850825950091399,0.855179842744098,0.859480037067308,0.86372588293052,0.867916738419968,0.87205196993568,0.876130952287265,0.880153068788438,0.884117711350248,0.888024280573021,0.891872185836974,0.895660845391512,0.899389686443183,0.903058145242268,0.906665667168023,0.910211706812523,0.913695728063121,0.917117204183505,0.920475617893327,0.923770461446416,0.927001236707534,0.930167455227694,0.933268638318005,0.936304317122042,0.93927403268673,0.942177336031735,0.945013788217338,0.947782960410804,0.95048443395121,0.95311780041274,0.955682661666441,0.958178629940404,0.960605327878401,0.962962388596925,0.965249455740667,0.967466183536386,0.969612236845188,0.971687291213196,0.973691032920597,0.97562315902908,0.977483377427628,0.979271406876687,0.980986977050686,0.9826298285789,0.984199713084672,0.985696393222957,0.987119642716209,0.988469246388591,0.989745000198504,0.990946711269438,0.992074197919133,0.993127289687043,0.99410582736011,0.995009662996835,0.995838659949645,0.99659269288555,0.997271647805093,0.997875422059586,0.998403924366628,0.998857074823906,0.999234804921275,0.999537057551115,0.999763787016967,0.99991495904044,0.999990550766393,0.999990550766393,0.99991495904044,0.999763787016967,0.999537057551115,0.999234804921275,0.998857074823906,0.998403924366628,0.997875422059586,0.997271647805093,0.99659269288555,0.995838659949645,0.995009662996835,0.99410582736011,0.993127289687043,0.992074197919133,0.990946711269438,0.989745000198504,0.988469246388591,0.987119642716209,0.985696393222957,0.984199713084672,0.9826298285789,0.980986977050686,0.979271406876687,0.977483377427628,0.97562315902908,0.973691032920598,0.971687291213196,0.969612236845189,0.967466183536386,0.965249455740667,0.962962388596925,0.960605327878401,0.958178629940405,0.955682661666441,0.95311780041274,0.95048443395121,0.947782960410804,0.945013788217338,0.942177336031735,0.93927403268673,0.936304317122042,0.933268638318005,0.930167455227694,0.927001236707534,0.923770461446416,0.920475617893328,0.917117204183505,0.913695728063121,0.910211706812523,0.906665667168023,0.903058145242268,0.899389686443183,0.895660845391512,0.891872185836974,0.888024280573021,0.884117711350248,0.880153068788438,0.876130952287266,0.87205196993568,0.867916738419968,0.86372588293052,0.859480037067308,0.855179842744099,0.850825950091399,0.84641901735817,0.841959710812305,0.837448704639903,0.832886680843338,0.828274329138148,0.823612346848765,0.818901438803083,0.814142317225904,0.809335701631252,0.804482318713599,0.799582902237994,0.794638192929131,0.789648938359361,0.784615892835666,0.779539817285623,0.77442147914236,0.769261652228528,0.764061116639314,0.7588206586245,0.753541070469591,0.748223150376032,0.742867702340537,0.737475536033525,0.73204746667672,0.726584314919893,0.721086906716794,0.715556073200279,0.709992650556654,0.704397479899253,0.698771407141278,0.693115282867903,0.687429962207682,0.681716304703259,0.675975174181427,0.670207438622517,0.664413970029181,0.658595644294553,0.652753341069825,0.646887943631258,0.641000338746643,0.635091416541232,0.629162070363163,0.6232131966484,0.617245694785205,0.611260466978157,0.605258418111759,0.599240455613625,0.593207489317293,0.587160431324672,0.581100195868139,0.575027699172326,0.568943859315596,0.56284959609124,0.556745830868423,0.550633486452881,0.544513486947404,0.538386757612132,0.532254224724658,0.526116815439995,0.5199754576504,0.513831079845091,0.507684610969869,0.501536980286678,0.49538911723311,0.489241951281889,0.483096411800347,0.476953427909915,0.470813928345655,0.464678841315845,0.45854909436165,0.452425614216887,0.446309326667919,0.440201156413684,0.434102026925899,0.42801286030944,0.421934577162933,0.415868096439574,0.40981433530819,0.403774209014585,0.397748630743159,0.391738511478851,0.385744759869409,0.379768282088018,0.373809981696295,0.367870759507684,0.361951513451266,0.356053138436005,0.350176526215452,0.344322565252915,0.338492140587147,0.332686133698534,0.326905422375828,0.321150880583437,0.315423378329297,0.309723781533332,0.304052951896545,0.29841174677074,0.292801019028898,0.287221616936238,0.281674384021968,0.276160158951759,0.270679775400947,0.265234061928494,0.259823841851719,0.254449933121828,0.249113148200245,0.243814293935788,0.238554171442674,0.233333575979408,0.228153296828549,0.223014117177384,0.217916813999514,0.212862157937393,0.207850913185816,0.20288383737638,0.197961681462944,0.193085189608094,0.188255099070633,0.183472140094124,0.17873703579648,0.174050502060644,0.169413247426353,0.164825972983019,0.160289372263735,0.15580413114042,0.151370927720124,0.146990432242509,0.14266330697852,0.138390206130252,0.134171775732054,0.130008653552846,0.125901468999704,0.121850843022703,0.117857388021034,0.113921707750418,0.11004439723183,0.106226042661535,0.102467221322469,0.0987685014969556,0.0951304423807942,0.091553593998715,0.0880384971212229,0.0845856831828409,0.0811956742017641,0.0778689827009387,0.0746061116305735,0.0714075542921033,0.068273794263606,0.0652053053266948,0.0622025513948854,0.0592659864434625,0.0563960544408427,0.0535931892814527,0.0508578147201306,0.0481903443080604,0.0455911813302486,0.0430607187445523,0.0405993391222702,0.0382074145903025,0.0358853067748913,0.0336333667469461,0.031451934968968,0.0293413412435765,0.0273019046636465,0.0253339335640675,0.0234377254751261,0.0216135670775249,0.0198617341590389,0.0181824915728217,0.0165760931973613,0.0150427818980994,0.0135827894907122,0.0121963367060628,0.0108836331568306,0.00964487730581992,0.00848025643595618,0.00738994662196968,0.00637411270377736,0.00543290826155973,0.00456647559254275,0.00377494568948344,0.00305843822086654,0.00241706151281168,0.00185091253269615,0.00136007687449447,0.000944628745838338,0.000604630956796859,0.000340134910380874,0.000151180594771427,0.0000377965772740962,0}

//Table 512 bufferSize
const double HANN[] = HANN_WINDOW_512;

///// EQUALIZER

  #define EQUALIZER_256 {1.00055558830634,1.00055558830634,1.00055558830634,1.26311772157691,1.26311772157691,1.26311772157691,1.23935047273031,1.23935047273031,1.23935047273031,1.27813189013084,1.27813189013084,1.27813189013084,1.30888676144959,1.30888676144959,1.30888676144959,1.34982514709464,1.34982514709464,1.34982514709464,1.50872106087913,1.50872106087913,1.50872106087913,1.63648667016512,1.63648667016512,1.63648667016512,1.9205764154514,1.9205764154514,1.9205764154514,2.30601690238367,2.30601690238367,2.30601690238367,2.48313310529557,2.49746838915414,2.49746838915414,2.67608559319933,2.7214093232028,2.7214093232028,2.71956848609892,2.71956848609892,2.71956848609892,2.78137725537687,2.81054977149375,2.83850905262673,2.86521296834092,2.89062100475819,2.91469436684605,2.9373960770238,2.95869106973541,2.97854628165103,2.99693073717236,3.01381562893261,3.02917439299833,3.04298277849824,3.05521891142387,3.06586335236679,3.07489914797942,3.08231187596854,3.08808968345442,3.09222331855278,3.09470615506176,3.09553421016174,3.09470615506176,3.09222331855278,3.08808968345442,3.08231187596854,3.07489914797942,3.08891596817531,3.07901512657619,3.09222331855278,3.07983898320448,3.09222331855278,3.10465745253109,3.11714158538459,3.12967591816369,3.14226065272722,3.1548959917457,3.13554250975987,3.11464074926508,3.09222331855278,3.10216663224483,3.11214191952902,3.12214928321921,3.13218882645989,3.10465745253109,3.11380758316775,3.12298468131755,3.09222331855278,3.1005071956796,3.10881326480653,3.11714158538459,3.12549221702406,3.13386521949491,3.14226065272722,3.15067857681163,3.15911905199975,3.17606789750112,3.17606789750112,3.24805567384253,3.24805567384253,3.24805567384253,3.18556281129451,3.18556281129451,3.18556281129451,3.11444494876854,3.11444494876854,3.11444494876854,3.494272830417,3.494272830417,3.494272830417,4.17567236193701,4.17567236193701,4.17567236193701,4.91177895349196,4.91177895349196,4.91177895349196,5.01702210863056,5.01702210863056,5.01702210863056,5.76631086264975,5.76631086264975,5.76631086264975,6.15525689892094,6.15525689892094,6.15525689892094,6.47489143062415,6.47489143062415,6.47489143062415,6.15168290023329,6.15168290023329,6.15168290023329,6.48764506910093,6.48764506910093,6.48764506910093,6.37051643858751,6.37051643858751,6.37051643858751,5.32349121247045,5.32349121247045,5.32349121247045,5.55849228657028,5.55849228657028,5.55849228657028,4.96840833204947,4.96840833204947,4.96840833204947,4.61347090193838,4.61347090193838,4.61347090193838,4.36849574315519,4.36849574315519,4.36849574315519,4.16929411958828,4.16929411958828,4.16929411958828,3.97805003449124,3.97805003449124,3.97805003449124,3.69933092400896,3.69933092400896,3.69933092400896,3.44223896219172,3.44223896219172,3.44223896219172,3.11390598828773,3.11390598828773,3.11390598828773,2.89731059200513,2.89731059200513,2.89731059200513,2.40178799286769,2.40178799286769,2.40178799286769,2.01989577237399,2.01989577237399,2.01989577237399,1.89321803449434,1.89321803449434,1.89321803449434,2.12786854517727,2.12786854517727,2.12786854517727,2.09627337121202,2.09627337121202,2.09627337121202,2.10383604816198,2.10383604816198,2.10383604816198,2.39312249770396,2.39312249770396,2.39312249770396,2.48745717051105,2.48745717051105,2.48745717051105,2.28828636312718,2.28828636312718,2.28828636312718,2.18901262416899,2.18901262416899,2.18901262416899,2.03678949129076,2.03678949129076,2.03678949129076,1.94468206372203,1.94468206372203,1.94468206372203,2.01000384729899,2.01000384729899,2.01000384729899,1.95610860341622,1.95610860341622,1.95610860341622,2.06708020801029,2.06708020801029,2.06708020801029,2.1539538260373,2.1539538260373,2.1539538260373,2.20834930374461,2.20834930374461,2.20834930374461,2.49240100850584,2.49240100850584,2.49240100850584,2.73724311413788,2.73724311413788,2.73724311413788,2.77082364577565,2.77082364577565,2.77082364577565,2.88283495134956,2.88283495134956,2.88283495134956,3.02544348343732,3.02544348343732,3.02544348343732,3.16103513124308,3.16103513124308,3.16103513124308,3.21245619070415,3.21245619070415,3.21245619070415,3.48925550704894,3.48925550704894,3.48925550704894,3.66447058329304,3.66447058329304,3.66447058329304,3.77505933612704,3.77505933612704,3.77505933612704,4.02145823649982,4.02145823649982,4.02145823649982,4.45882768322875,4.45882768322875,4.45882768322875,4.45882768322875}

//Table 512 bufferSize
const double EQUALTAB[] = EQUALIZER_256;

///// DEBUGING
const int PRIORITY = 5;
