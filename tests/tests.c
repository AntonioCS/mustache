/* 
 * File:   tests.c
 * Author: antoniocs
 *
 * Created on 2 de Novembro de 2014, 20:24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/mustache.h"
//#include "src/mustache_get_contents.h"
/*
char text_tag_variable[] = "Hello {{name}}";
char text_tag_variable_no_escape[] = "Escaped html: {{html}} ---- Not escaped html: {{{html}}}";
char text_tag_variable_no_escape_andsign[] = "Escaped html: {{html}} ---- Not escaped html: {{&html}}";
char text_tag_comment[] = "<h1>Today{{! ignore me }}.</h1>";
char text_tag_comment_multiline[] = "<h1>Today</h1> {{! ignore me\n"
                                    " This is still a commnent\n"
                                    " This is still a commnent2\n"
                                    " This is still a commnent3}}\n"
                                    "This should be seen -> {{ name}}";
char text_tag_variable_error[] = "Hello {{name}"; //missing end }
char text_tag_variable_error2[] = "Hello {name}}"; //Missing second {. This will not generate an error the parser is suppose to just show everything


char text_tag_section[] = "Hello {{name}}\n"
               "You have just won {{value}} dollars!\n"
               "{{#in_ca}}"
               "Well, {{taxed_value}} dollars, after taxes.\n"
               "{{/in_ca}}"
               "The End"
        ;

char text_tag_section_inverted[] = "Hello {{name}}\n"
               "You have just won {{value}} dollars!\n"
               "{{#in_ca}}"
               "Well, {{taxed_value}} dollars, after taxes.\n"
               "{{/in_ca}}"
               "{{^in_ca}}"
               "No tax!!\n"
               "{{/in_ca}}"
               "The End"
        ;

char text_big[] = "Lorem ípsum dôlor sit àmet, in êligéndí scâévolà definitíonem cum. Intéllégam suscipiãntur át cum, prô eripuít mentítum assêntior ne. Éi cum indoçtum dissêntias. Díçát dolôrem ullâmcorper seá ex.\n"
"Usu laúdem eúripídís ex. Possít ôbliqúe in sít. Pósse quodsi cívibus ex mei. Débet mundi pro {{value}} êu, meí tímeám sánctus {{value}} eu. No nãtum áffert invenire eam. In viris dôlôrum philosophiá qúó, vêl brúte eleifénd cônçludátúrque id.\n"
"Quo habéo cótidíêqúe àt, hàrum málorum elêctram ut úsu. Êós in sempér adversárium, êx quaeque epicurei nàm. Id brúté regióne cúm, {{value}} vix sint sânçtus accumsan eá. Vidit súmmo sit te, malis mollis lúptátum eum in.\n"
"Eam id ígnota suavitàte {{value}} labóràmus. Sea éú mnesarchúm thêõphrástus, êi his novúm sánctus pêrtinàcia. Neç êt inàní salutàtus cotidíêquê. Eum ât invenire adipiscing. Vím vidissé sadipscing cu, etiam repudiãndáé vís no. Ancílláe ôffendit démócritum ea ius, ut probo ludús pàrtem sit.\n"
"No patrioque êuripídis eõs, mea mõvet áppetere pércípit eú, vix éránt mãluissêt te. Meâ nonumy ófficiis êa. Nó suscipit principes tincidunt pri, elít meís côngue mea ut. Nê pri minim luptàtúm pericúlà, id sêd nõstrum côtidieque. Feúgaít álbucius íudíçâbit ut vel.\n"
"At úsu vêniam {{name}} tempor rãtiónibus. Sea no eírmod{{name}}minimum, sed no legére voçibus. Utínàm dõctus facilisis te ius, noluisse platoném ius ei, úsú tê putent suàvitàté. Impetus ãssentiôr expetendís ut ius, vidít elàboraret at est. No mel soleàt abhõrreànt. Hís tollít graeci definiebas ãd.\n"
"An per inani prôpriaê hendrérit. Ex qúi tãntás detérruisset, ne vidé õmittântur néc, môdo integre fuisset eí sea. Hás dicit errem pátrioqúe êí, qui lâbores suàvitate no. Usu libris dôlores êu, cu réqúe mucíús euripidis éum.\n"
"At vérêàr definiebas méà. Êi per consúl intêgre, elêctram  {{name}} àcçusamus tínçídunt id prí, mutat sôlet épiçúrí mel et. Dolóre éirmod te nàm, ei primis ànimal pri. In dúis ãtquí qui, quot dignissim vel çu.\n"
"At vérêàr definiebas méà. Êi per consúl intêgre, elêctram  àcçusamus tínçídunt id prí, mutat sôlet épiçúrí mel et. Dolóre éirmod te nàm, ei primis ànimal pri. In dúis ãtquí qui, quot dignissim vel çu.\n"
"Diço cõncéptam vis te, iriurê moléstiáe âlíquàndó per no. Eú sed âppetêré abhorrêãnt. Pro sólêt expeténdis detérrúisset té. Quo ex fãllí âéquê perseqúeris. Ea eros cômmodo làbítur sed, vis ei quem doctus õmnesque, ei mél ullum tráçtátós reprehendúnt. Õmnium disputândô eum ex.\n"
"Ut duô ãequé deliçatâ, {{value}} in cum natum intêgre sanctus, ut cum qúem mollis. Impedit máiorum cónçeptam his át, âmet prodésset persecuti mea âd. Quo ât tôtã cónsétetúr sçribentúr, nám et zril êquídem, át meí âdhuc ináni. Harum eúismod ât nám, ornàtus ullàmcôrper te usu, vólutpat evértitúr per ut. Neç lãbore cótidieque íd.\n"
;

char text_tag_delimiter[] = "Hello {{name}}\n"
               "{{=<% %>=}} You have just won {{value}} dollars!\n"               
               "Well, <%taxed_value%> dollars, after taxes.\n"               
               "The End"
        ;
*/
static pmustache mustache_instance;

#define CALL_TEST_FUNC(test_func, name) \
    { \
    int state = 0;\
    set_up(); \
    printf("TEST: %s", name); \
    if (test_func()) { \
        printf(" PASSED\n"); \
        state = 1;\
    } else {\
        printf(" FAILED ###############################\n"); \
    }\
    tear_down();\
    if (state == 0) exit(-1);\
    }
    
#define M_SET_LOAD_CALL(tag, tag_value, string, correct_string)\
    mustache_set(mustache_instance, tag, tag_value);\
    mustache_load_txt(mustache_instance, string);\
    mustache_render(mustache_instance);\
    return (strcmp(correct_string, mustache_instance->text_parsed) == 0); 


void set_up() {
    mustache_instance = mustache_init();
}

void tear_down() {
    
}

static int test_init() {
    return (mustache_instance != NULL);
}

static int test_tag_variable() {
    M_SET_LOAD_CALL("name", "António", "Hello {{name}}", "Hello António")
}

//Tag content is escaped by default
static int test_tag_escaped() {
    M_SET_LOAD_CALL("escape_chars", "&\"'<>", "{{escape_chars}}", "&amp;&quot;&apos;&lt;&gt;")
}

static int test_tag_not_escaped() {
    M_SET_LOAD_CALL("not_escape_chars", "&\"'<>", "{{{not_escape_chars}}}", "&\"'<>")    
}

static int test_tag_not_escaped_with_and_sign() {
    M_SET_LOAD_CALL("not_escape_chars", "&\"'<>", "{{{not_escape_chars}}}", "&\"'<>")    
}

static int test_tag_comment() {
    M_SET_LOAD_CALL("not_used", "not_used", "Hello{{! ignore me }}!!", "Hello!!")
}

static int test_tag_multiline_comment() {
    char *m_comment = "Hello{{! ignore me\n"
                        " This is still a commnent\n"
                        " This is still a commnent2\n"
                        " This is still a commnent3}}\n"
                        " This should be seen!";    
    M_SET_LOAD_CALL("not_used", "not_used", m_comment, "Hello\n This should be seen!")
}

//static int test_tag_name_error() {
    //mustache_set(mustache_instance, "name", "acs");
    //mustache_load_txt(mustache_instance, "Hello {{name}");
    //mustache_render(mustache_instance);
    //return (strcmp(correct_string, mustache_instance->text_parsed) == 0); 
    //M_SET_LOAD_CALL("name", "Acs", "Hello {{name}", "Hello {{name}")
//}



static int test_tag_section() {
    char *m_section =   "Text text\n"
                        "{{#in_ca}}"
                            "In section text\n"
                        "{{/in_ca}}"
                        "The End";
    
    M_SET_LOAD_CALL("in_ca", "true", m_section,  "Text text\nIn section text\nThe End")
}

char text_tag_section[] = "Hello {{name}}\n"
               "You have just won {{value}} dollars!\n"
               "{{#in_ca}}"
               "Well, {{taxed_value}} dollars, after taxes.\n"
               "{{/in_ca}}"
               "The End"
        ;

char text_tag_section_inverted[] = "Hello {{name}}\n"
               "You have just won {{value}} dollars!\n"
               "{{#in_ca}}"
               "Well, {{taxed_value}} dollars, after taxes.\n"
               "{{/in_ca}}"
               "{{^in_ca}}"
               "No tax!!\n"
               "{{/in_ca}}"
               "The End"
        ;

//char text_tag_variable_error[] = "Hello {{name}"; //missing end }
//char text_tag_variable_error2[] = "Hello {name}}"; //Missing second {. This will not generate an error the parser is suppose to just show everything

int main(int argc, char **argv) {
    CALL_TEST_FUNC(test_init, "Initiate")
    CALL_TEST_FUNC(test_tag_variable, "Tag variable replace")
    CALL_TEST_FUNC(test_tag_escaped, "Tag variable escape")
    CALL_TEST_FUNC(test_tag_not_escaped, "Tag variable not escaped using {{{ }}}")
    CALL_TEST_FUNC(test_tag_not_escaped_with_and_sign, "Tag variable not escaped using &")
    CALL_TEST_FUNC(test_tag_comment, "Tag comment")
    CALL_TEST_FUNC(test_tag_multiline_comment, "Tag comment multi line")
    //CALL_TEST_FUNC(test_tag_name_error, "Tag error missing end }")
            
    CALL_TEST_FUNC(test_tag_section, "Tag section")
    
/*
   
    //text = text_tag_variable_error;
    //text = text_tag_variable_error2;
    //text = text_tag_section;
    //text = text_tag_section_inverted;
    //text = text_big;
    text = text_tag_delimiter;

    mustache_set(m, "name", "António");
    mustache_set(m, "value", "123.33");
    mustache_set(m, "taxed_value", "50");
    mustache_set(m, "html", "<script type=\"text/javascript\" src=\"acs.js\" />");
    //mustache_set(m, "in_ca", "true");

    mustache_load_txt(m, text);

    mustache_render(m);

    printf("Rendered text: %s\n", m->text_parsed);
//*/
  /*
    char *text = NULL;

    text = mustache_get_contents("src/tpl.mustache");
    //text = mustache_get_contents("src/tpl_large.mustache");
    printf("Text: %s\n", text);
    free(text);
*/
    
    return (EXIT_SUCCESS);
}



