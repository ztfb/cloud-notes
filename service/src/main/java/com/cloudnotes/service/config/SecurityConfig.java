package com.cloudnotes.service.config;

import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;

@Configuration
@EnableWebSecurity
public class SecurityConfig extends WebSecurityConfigurerAdapter {
    @Override
    protected void configure(HttpSecurity http) throws Exception {
        //关闭csrf防护
        http.csrf().disable();
        // 解决跨域问题
        http.cors();
        //不通过Session获取SecurityContext
        http.sessionManagement().sessionCreationPolicy(SessionCreationPolicy.STATELESS)
                .and()
                .authorizeRequests()                                //授权配置
                //登录路径放行
                .antMatchers("/**")
                .permitAll()
                .anyRequest().authenticated()   ;                //其他路径都要认证之后才能访问
    }
}
