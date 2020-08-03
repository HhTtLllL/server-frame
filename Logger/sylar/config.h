#ifndef __SYLAR_CONFIG_H__
#define __SYLAR_CONFIG_H__

#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>
#include "./log.h"
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>



namespace sylar
{

//配置变量的基类
class ConfigVarBase{
	
public:
	typedef std::shared_ptr<ConfigVarBase> ptr;

	ConfigVarBase(const std::string& name, const std::string& description = "")
		:m_name(name),
		m_description(description) {
			std::transform(m_name.begin(),m_name.end(),m_name.begin(),::tolower);

		}

	virtual ~ConfigVarBase() {}

	//返回配置参数与的名称
	const std::string& getName() const { return m_name; }
	//返回配置参数的描述
	const std::string& getDescription() const { return m_description; }

	//转成字符串
	virtual std::string toString() = 0; //序列化
	//从字符串初始化值
	virtual bool fromString (const std::string& val) = 0;//反序列化


protected:
	std::string m_name;//配置名称
	std::string m_description; //配置描述

};

//F from_type, T to_type
template<class F, class T>
class LexicalCast{
public:
	T operator() (const F& v){
		return boost::lexical_cast<T>(v);
	}

};

//支持 vector
template<class T>
class LexicalCast<std::string, std::vector<T> >{
public:
	std::vector<T> operator() (const std::string& v){
		YAML::Node node = YAML::Load(v);

		typename std::vector<T> vec;
		std::stringstream ss;

		for(size_t i = 0; i < node.size() ; i ++ ){
			ss.str("");
			ss << node[i];
			vec.push_back(LexicalCast<std::string, T> () (ss.str()));
		}

		return vec;
	}

};

template<class T>
class LexicalCast<std::vector<T> , std::string>{
public:
	std::string operator() (const std::vector<T>& v){
		YAML::Node node;

		for(auto& i : v){
			node.push_back(YAML::Load(LexicalCast<T, std::string>() (i)));
		}

		std::stringstream ss;
		ss << node;

		return ss.str();
	}

};

//支持list
template<class T>
class LexicalCast<std::string, std::list<T> >{
public:
	std::list<T> operator() (const std::string& v){
		YAML::Node node = YAML::Load(v);

		typename std::list<T> vec;
		std::stringstream ss;

		for(size_t i = 0; i < node.size() ; i ++ ){
			ss.str("");
			ss << node[i];
			vec.push_back(LexicalCast<std::string, T> () (ss.str()));
		}

		return vec;
	}

};

template<class T>
class LexicalCast<std::list<T> , std::string>{
public:
	std::string operator() (const std::list<T>& v){
		YAML::Node node;

		for(auto& i : v){
			node.push_back(YAML::Load(LexicalCast<T, std::string>() (i)));
		}

		std::stringstream ss;
		ss << node;
		
		return ss.str();
	}

};

//支持set
template<class T>
class LexicalCast<std::string, std::set<T> >{
public:
	std::set<T> operator() (const std::string& v){
		YAML::Node node = YAML::Load(v);

		typename std::set<T> vec;
		std::stringstream ss;

		for(size_t i = 0; i < node.size() ; i ++ ){
			ss.str("");
			ss << node[i];
			vec.insert(LexicalCast<std::string, T> () (ss.str()));
		}

		return vec;
	}

};

template<class T>
class LexicalCast<std::set<T> , std::string>{
public:
	std::string operator() (const std::set<T>& v){
		YAML::Node node;

		for(auto& i : v){
			node.push_back(YAML::Load(LexicalCast<T, std::string>() (i)));
		}

		std::stringstream ss;
		ss << node;
		
		return ss.str();
	}

};

//  支持unordered_set
template<class T>
class LexicalCast<std::string, std::unordered_set<T> >{
public:
	std::unordered_set<T> operator() (const std::string& v){
		YAML::Node node = YAML::Load(v);

		typename std::unordered_set<T> vec;
		std::stringstream ss;

		for(size_t i = 0; i < node.size() ; i ++ ){
			ss.str("");
			ss << node[i];
			vec.insert(LexicalCast<std::string, T> () (ss.str()));
		}

		return vec;
	}

};

template<class T>
class LexicalCast<std::unordered_set<T> , std::string>{
public:
	std::string operator() (const std::unordered_set<T>& v){
		YAML::Node node;

		for(auto& i : v){
			node.push_back(YAML::Load(LexicalCast<T, std::string>() (i)));
		}

		std::stringstream ss;
		ss << node;
		
		return ss.str();
	}

};

//  支持map
template<class T>
class LexicalCast<std::string, std::map<std::string, T> >{
public:
	std::map<std::string, T> operator() (const std::string& v){
		YAML::Node node = YAML::Load(v);

		typename std::map<std::string, T> vec;
		std::stringstream ss;

		for(auto it = node.begin() ; 
				it != node.end() ; ++it){
			ss.str("");
			ss << it->second;
			vec.insert(std::make_pair(it->first.Scalar(), LexicalCast<std::string, T>() (ss.str())));
		}

		return vec;
	}

};

template<class T>
class LexicalCast<std::map<std::string, T> , std::string>{
public:
	std::string operator() (const std::map<std::string, T>& v){
		YAML::Node node;

		for(auto& i : v){
			node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
		}

		std::stringstream ss;
		ss << node;
		
		return ss.str();
	}

};


//  支持umap
template<class T>
class LexicalCast<std::string, std::unordered_map<std::string, T> >{
public:
	std::unordered_map<std::string, T> operator() (const std::string& v){
		YAML::Node node = YAML::Load(v);

		typename std::unordered_map<std::string, T> vec;
		std::stringstream ss;

		for(auto it = node.begin() ; 
				it != node.end() ; ++it){
			ss.str("");
			ss << it->second;
			vec.insert(std::make_pair(it->first.Scalar(), LexicalCast<std::string, T>() (ss.str())));
		}

		return vec;
	}

};

template<class T>
class LexicalCast<std::unordered_map<std::string, T> , std::string>{
public:
	std::string operator() (const std::unordered_map<std::string, T>& v){
		YAML::Node node;

		for(auto& i : v){
			node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
		}

		std::stringstream ss;
		ss << node;
		
		return ss.str();
	}

};




//配置项的具体实现类
template<class T, class FromStr = LexicalCast<std::string,T>,
	class ToStr = LexicalCast<T,std::string> >
class ConfigVar : public ConfigVarBase{

public:
	typedef std::shared_ptr<ConfigVar> ptr;

	ConfigVar(const std::string& name, const T& default_value,const std::string& description = "")
		: ConfigVarBase(name,description), m_val(default_value){
		
		}

	std::string toString () override{
	
		try{
			//在这里捕捉类型转化的异常 把m_val 类型转化为string 类型
			//return boost::lexical_cast<std::string>(m_val); //类型转化
			return ToStr() (m_val);
		}catch(std::exception& e){
			SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "configVar::toString exception" << e.what() << " convert: " << typeid(m_val).name() << "to string";

		}

		return "";
	}


	bool fromString(const std::string& val) override {
		
		try{
			//把string 类型转换为我们需要的类型 模板T 类型
			//m_val = boost::lexical_cast<T>(val);
			setValue(FromStr() (val));

		}catch(std::exception& e){
			SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "configVar::toString exception" << e.what() << " convert: " << typeid(m_val).name() << "to string";

		}


		return false;
	}

	const T getValue() const { return m_val; }
	void setValue(const T& val) { m_val = val ; } 
private:
	T m_val;





};

//ConfigVar 的集合类
class Config{

public:

	typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

	template<class T>
	static typename ConfigVar<T>::ptr Lookup(const std::string& name,
			const T& default_value, const std::string& description = ""){
		auto tmp = Lookup<T>(name);
		if(tmp){
			SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "Lookup name" << name << "exists";
			return tmp;
		}

		if(name.find_first_not_of("abcdefghigklmnopqrstuvwxyz._0123456789") != std::string::npos){
			SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name invalid " << name;
			throw std::invalid_argument(name);
		
		}

		typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value,description));
		s_datas[name] = v;

		return v;
	}

	template<class T>
	static typename ConfigVar<T>::ptr Lookup(const std::string& name){
		auto it = s_datas.find(name);
		if(it == s_datas.end()){
			return nullptr;
		}

		return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
	
	}

	static void LoadFromYaml(const YAML::Node& root);
	static ConfigVarBase::ptr LookupBase(const std::string& name);



private:
	static ConfigVarMap s_datas;

};










}





#endif
