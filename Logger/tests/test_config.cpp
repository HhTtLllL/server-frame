//##################################################################
// File Name: test_config.cpp
// File Effect: 
// Author: tttt
// mail: tttt@xiyoulinux.org
// Created Time: 2020年08月01日 星期六 10时28分20秒
//=============================================================
//!/usr/bin/python

#include "../sylar/config.h"
#include "../sylar/log.h"
#include <yaml-cpp/yaml.h>
#include <iostream>


//约定好的配置
sylar::ConfigVar<int>::ptr g_int_value_config = 
	sylar::Config::Lookup("system.port",(int)8080,"system port");

sylar::ConfigVar<float>::ptr g_float_value_config = 
	sylar::Config::Lookup("system.value",(float)10.2f,"system value");

sylar::ConfigVar<std::list<int> >::ptr g_int_list_value_config = 
	sylar::Config::Lookup("system.int_list",std::list<int>{1,2},"system int_vec");


sylar::ConfigVar<std::vector<int> >::ptr g_int_vec_value_config = 
	sylar::Config::Lookup("system.int_vec",std::vector<int>{1,2},"system int_vec");

sylar::ConfigVar<std::set<int> >::ptr g_int_set_value_config = 
	sylar::Config::Lookup("system.int_set",std::set<int>{1,2},"system int_set"); 

sylar::ConfigVar<std::unordered_set<int> >::ptr g_int_uset_value_config = 
	sylar::Config::Lookup("system.int_uset",std::unordered_set<int>{1,2},"system int_uset");

sylar::ConfigVar<std::map<std::string, int> >::ptr g_str_int_map_value_config = 
	sylar::Config::Lookup("system.str_int_map",std::map<std::string, int>{{"k",2}},"system str_int_map");

//遍历yaml 
void print_yaml(const YAML::Node& node,int level){
	
	if(node.IsScalar()){
		SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level *4,' ') << 
			node.Scalar() << " - " << node.Type() << " - " << level;
	}
	else if(node.IsNull()){
		SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level* 4,' ') 
				<< "NULL - " << node.Type() << " - " << level;
		//是map结构的话
	}else if(node.IsMap()){
		for(auto it = node.begin() ; it != node.end() ; ++ it)
		{
			SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level * 4,' ')
				<< it->first << " - " << it->second.Type() << " - " << level;

			print_yaml(it->second, level + 1);
		}
	}else if(node.IsSequence()){
		for(size_t i = 0; i < node.size(); ++ i){
			SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level * 4,' ')
				<< i << " - " << node[i].Type() << " - " << level;
			print_yaml(node[i],level + 1);
		}
	}
}
void test_yaml(){

	YAML::Node root = YAML::LoadFile("/home/tt/server-frame/Logger/bin/conf/log.yaml");
	print_yaml(root, 0);
	//打印 加载进去的文件
	//SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << root;
}


void test_config(){


#define XX(g_var, name, prefix) \
	{\
		auto& v = g_var->getValue(); \
		for(auto& i : v){ \
			SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) <<  #prefix " " #name " : " << i; \
		}\
		SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) <<  #prefix " " #name " yaml : " << g_var->toString(); \
	}

#define XX_M(g_var, name, prefix) \
	{\
		auto& v = g_var->getValue(); \
		for(auto& i : v){ \
			SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << #prefix " " #name ": {"\
			<<i.first << " - " << i.second << "}"; \
		}\
		SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) <<  #prefix " " #name " yaml : " << g_var->toString(); \
	}

	XX(g_int_vec_value_config, int_vec, before);
	XX(g_int_list_value_config, int_list, before);
	XX(g_int_set_value_config, int_set, before);
	XX(g_int_uset_value_config, int_uset, before);
	XX_M(g_str_int_map_value_config, str_int_map, before);


	
	YAML::Node root = YAML::LoadFile("/home/tt/server-frame/Logger/bin/conf/log.yaml");
	sylar::Config::LoadFromYaml(root);
	
	SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "after" << g_int_value_config->getValue();

	SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "after" << g_float_value_config->toString();

	XX(g_int_vec_value_config, int_vec, after);
	XX(g_int_list_value_config, int_list, after);
	XX(g_int_set_value_config, int_set, after);
	XX(g_int_uset_value_config, int_uset, after);
	XX_M(g_str_int_map_value_config, str_int_map,after);
	
}

int main()
{

	//test_yaml();
	test_config();



	return 0;
}
