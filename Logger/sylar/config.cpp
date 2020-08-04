//##################################################################
// File Name: config.cpp
// File Effect: 
// Author: tttt
// mail: tttt@xiyoulinux.org
// Created Time: 2020年08月01日 星期六 10时29分25秒
//=============================================================
//!/usr/bin/python

#include "./config.h"

namespace sylar{

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");
//Config::ConfigVarMap Config::s_datas;

ConfigVarBase::ptr Config::LookupBase(const std::string& name){

	auto it = GetDatas().find(name);

	return it == GetDatas().end() ? nullptr : it->second;
}

static void ListAllMember(const std::string& prefix,
		 	const YAML::Node& node,
			std::list<std::pair<std::string, const YAML::Node> >& output){
	if(prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyz._1234567890")
			!= std::string::npos){ //find 函数在找不到指定值的情况下 会返回string::npos
	
		SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Config invalid name: " << prefix << " : " << node;

		return ;
	}

	output.push_back(std::make_pair(prefix,node));
	if(node.IsMap()){
	
		for(auto it = node.begin();
				it != node.end() ; ++it){
		
			ListAllMember(prefix.empty() ? it->first.Scalar()
					: prefix + "." + it->first.Scalar(), it->second, output);
		}
	}


}


//使用 node 初始化配置模块
void Config::LoadFromYaml(const YAML::Node& root){
	std::list<std::pair<std::string, const YAML::Node> > all_nodes;
	ListAllMember("", root, all_nodes);

	for(auto& i : all_nodes){
	
		std::string key = i.first;
		if(key.empty()){
			continue;
		}


		std::transform(key.begin(), key.end() ,key.begin(), ::tolower);
		ConfigVarBase::ptr  var = LookupBase(key);

		if(var){
			//Scalar  纯量    存量是最基本的、不可再分的值。 以下数据类型都属于 纯量
																											// 字符串、布尔、整数、浮点数、NULL、时间、日期
			if(i.second.IsScalar()){   //如果是基本类型就直接转化为 字符串
				var->fromString(i.second.Scalar());
			}else{
				std::stringstream ss;
				ss << i.second;
				var->fromString(ss.str());
			}
		}
	}
}


}
