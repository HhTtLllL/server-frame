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
/*
		ConfigVarBase 是具体的基类，ConfigVar 是一个配置项的具体实现，Config 是配置项的集合


*/
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

	//返回配置参数的名称
	const std::string& getName() const { return m_name; }
	//返回配置参数的描述
	const std::string& getDescription() const { return m_description; }

	//转成字符串
	virtual std::string toString() = 0; //序列化
	//从字符串初始化值
	virtual bool fromString (const std::string& val) = 0;//反序列化
	virtual std::string getTypeName() const  = 0;

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
//T 是配置项代表的具体类型。 getValue 方法返回对应类型的实例
// FromStr  实现将string 转化为 T 的功能
// ToStr: 实现将T 转化为string 的功能
//
template<class T, class FromStr = LexicalCast<std::string,T>,
	class ToStr = LexicalCast<T,std::string> >
class ConfigVar : public ConfigVarBase{

public:
	typedef std::shared_ptr<ConfigVar> ptr;

	/*通过参数名，参数值，描述，构造ConfigVar
	 *name 参数名称有效字符为[0-9,a-z,_.]
	 *default_value 参数的默认值
	 *
	 * description 参数的描述
	 * */
	ConfigVar(const std::string& name, const T& default_value,const std::string& description = "")
		: ConfigVarBase(name,description), m_val(default_value){
		
		}
	/*将参数值转换成YAML String 类型,当转换失败抛出异常
	 * */
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

	/*从YAML String 转成参数的值
	 * 当转换失败时，抛出异常
	 * */
	bool fromString(const std::string& val) override {
		
		try{
			//把string 类型转换为我们需要的类型 模板T 类型
			//m_val = boost::lexical_cast<T>(val);
			//class FromStr = lexcal_cast<std::string, T>
			setValue(FromStr() (val));

		}catch(std::exception& e){
			SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "configVar::toString exception" << e.what() << " convert: " << typeid(m_val).name() << "to string";

		}


		return false;
	}
	//获取当前参数的值
	const T getValue() const { return m_val; }
	//设置当前参数 的值
	void setValue(const T& val) { m_val = val ; } 
	std::string getTypeName() const override { return typeid(T).name(); }
private:
	T m_val;
};

//ConfigVar 的集合类
//配置集合类，提供所有配置项的ConfigVar 的统一管理功能
//加载配置文件，更新配置文件，定义配置项等等
class Config{

public:

	typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

	/*创建和定义配置项，同一个配置向只能创建一次，否则会冲突
	 *
	 *获取/创建对一个参数名的配置参数
	 *name : 配置参数名称
	 *default_value 参数默认值
	 *description  参数描述
	 *获取参数名为name 的配置参数，如果存在直接返回，如果不存在，创建参数配置并用 default_value 赋值
	 *
	 * 返回对应的配置参数，如果参数名存在但是类型不匹配则返回nullptr
	 *
	 * 如果参数名包括非法字符，则抛出异常
	 *
	 * */
	template<class T>
	static typename ConfigVar<T>::ptr Lookup(const std::string& name,
			const T& default_value, const std::string& description = ""){
		auto it = s_datas.find(name);
		if(it != s_datas.end()){
			auto tmp = std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
			if(tmp){
				SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "Lookup name" << name << "exists";
				return tmp;
			}else{
		
				SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << " LookUp name = " << name << "existe but type not" << typeid(T).name() << "real_type = " << it->second->getTypeName() << " " << it->second->toString();  
				return nullptr;
			}
		}
		// 检查名称中是否包含非法字符,有则抛出异常
		if(name.find_first_not_of("abcdefghigklmnopqrstuvwxyz._0123456789") != std::string::npos){
			SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name invalid " << name;
			throw std::invalid_argument(name);
		}

		//符合以上条件则 向map容器中插入该值
		typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value,description));
		s_datas[name] = v;

		return v;
	}
	
	//获取对应名称的配置项，用来获取上面函数创建的配置项
	/*
			查找配置参数
			name 配置参数名称
			返回配置参数名为name 的配置参数, 没找到则返回 nullptr
	*/
	template<class T>
	static typename ConfigVar<T>::ptr Lookup(const std::string& name){
		auto it = s_datas.find(name);
		if(it == s_datas.end()){
			return nullptr;
		}

		return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
	}
	//加载配置文件，加载一个目录下面的所有YAML 文件，支持多文件，不同的配置项可以分开文件定义
	//YAML::Node 初始化配置模块
	static void LoadFromYaml(const YAML::Node& root);
	
	/*查找配置参数,返回配置参数的基类
	 * name 配置参数名称
	 *
	 * */
	static ConfigVarBase::ptr LookupBase(const std::string& name);

private:
	static ConfigVarMap s_datas;

};

}
#endif
