#include <yajl/yajl_tree.h>

char* cmd = "{\"AddContact\":{\"Name\":\"name1\",\"Ip\":\"152.17.1.1:4242\",\"Key\":\"key\"}}";

char* cmd2 = "[{\"AddContact\":{\"Name\":\"name1\",\"Ip\":\"152.17.1.1:4242\",\"Key\":\"key\"}},{\"AddContact\":{\"Name\":\"name2\",\"Ip\":\"152.17.1.2:4242\",\"Key\":\"key2\"}}]";

#include <stdio.h>
#include <string.h>

static void    treat_array(yajl_val node);
static void    treat_object(yajl_val node);

static void    treat_string(yajl_val node)
{
    fprintf(stderr, "string value : %s\n", node->u.string);
}

static void    treat_object(yajl_val node)
{
    int i;

    fprintf(stderr, "start object\n");
    for (i = 0; i < node->u.object.len; ++i)
        {
            fprintf(stderr, "key == %s\n", node->u.object.keys[i]);
            if (YAJL_IS_OBJECT(node->u.object.values[i]))
                {
                    treat_object(node->u.object.values[i]);
                }
            else if (YAJL_IS_ARRAY(node->u.object.values[i]))
                {
                    treat_array(node->u.object.values[i]);
                }
            else if (YAJL_IS_STRING(node->u.object.values[i]))
                {
                    treat_string(node->u.object.values[i]);
                }
            else
                fprintf(stderr, "Error, type ununderstandable\n");
        }
    fprintf(stderr, "end object\n");
}

static void    treat_array(yajl_val node)
{
    int i;

    fprintf(stderr, "start array\n");
    for (i = 0; i < node->u.array.len; ++i)
        {
            if (YAJL_IS_OBJECT(node->u.array.values[i]))
                {
                    treat_object(node->u.array.values[i]);
                }
            else if (YAJL_IS_ARRAY(node->u.array.values[i]))
                {
                    treat_array(node->u.array.values[i]);
                }
            else if (YAJL_IS_STRING(node->u.array.values[i]))
                {
                    treat_string(node->u.object.values[i]);
                }
            /* else if (YAJL_IS_NULL(node->u.array.values[i])) */
            /*     { */
            /*         fprintf(stderr, "wesh, un null\n"); */
            /*     } */
            else
                fprintf(stderr, "Error, type ununderstandable\n");
        }
    fprintf(stderr, "end array\n");
}

int main()
{
    yajl_val    node;
    char* yajl_cmd = cmd;
    char errbuf[4096] = {0};
    int i;

    node = yajl_tree_parse(cmd2, errbuf, sizeof(errbuf));
    if (node == NULL)
        {
            if (strlen(errbuf))
                fprintf(stderr, " %s", errbuf);
            return 1;
        }

    if (YAJL_IS_ARRAY(node))
        {
            treat_array(node);
        }
    else if (YAJL_IS_OBJECT(node))
        {
            treat_object(node);
        }
    yajl_tree_free(node);
    return 0;
}
