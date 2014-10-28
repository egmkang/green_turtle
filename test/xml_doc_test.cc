#include <xml_doc_cache.h>

using namespace green_turtle;

int main() {
  XmlDocCache::Instance().LoadFile("file_name.txt");
  XmlDocCache::Instance().Clear();
  XmlDocCache::Instance().Clear("a");


  XmlDocCache::Instance().ForEach("file_name.txt", "/root/nodes",
                                    [](XmlDocCache::node_type node) {
    (void)node;
    return true;
  });

  XmlDocCache::Instance().First("file_name.txt", "/root/nodes",
                                    [](XmlDocCache::node_type node) {
    (void)node;
    return true;
  });


  return 0;
}
